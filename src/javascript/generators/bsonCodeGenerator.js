/*
    RULES:
        THERE CANNOT BE ARRAYS DIRECTLY INSIDE OTHER ARRAYS
*/

class BsonCodeGenerator {

    _getDepth(structure = this.structure) {
        let res;
        if (Array.isArray(structure)) {
            res = 1 + this._getDepth(structure[0]);
        }
        else if (typeof structure === 'object') {
            let max = 0;
            for (const key in structure) {
                if (typeof structure[key] === 'object') {
                    const n = 1 + this._getDepth(structure[key]);
                    max = (n > max) ? n : max;
                }
            }
            res = max;
        }
        else {
            res = 0;
        }
        return res;
    }

    _parseDepth() {
        return (this.depth === 0 ? '*sending' : `&children[${this.depth - 1}]`);
    }

    _parseKey({ key, type }) {
        switch (type) {
            case 'array':
                return `[${key}]`;
            case 'object':
                return `.${key}`;
        }
    }

    _parseKeys() {
        return this.keys.reduce((accumulator, current) => accumulator += this._parseKey(current), '').replace('.', '->');
    }

    _parseForDepth() {
        switch (this.forDepth) {
            case 0:
                return 'i';
            case 1:
                return 'j';
            case 2:
                return 'k';
            default:
                return `k${this.forDepth - 3}`;
        }
    }

    _getKey() {
        const last = this.keys[this.keys.length - 1];
        return last.type === 'array' ? '0' : last.key;
    }

    _tabsFromIndentation() {
        let res = '';
        for (let i = 0; i < this.indentation; i++) {
            res += '\t';
        }
        return res;
    }

    _parsePrimitive(data) {
        switch (data) {
            case 'int':
                this._print(`BSON_APPEND_INT32(${this._parseDepth()}, "${this._getKey()}", data${this._parseKeys()});`);
                break;
            case 'long':
                this._print(`BSON_APPEND_INT64(${this._parseDepth()}, "${this._getKey()}", data${this._parseKeys()});`);
                break;
            case 'double':
                this._print(`BSON_APPEND_DOUBLE(${this._parseDepth()}, "${this._getKey()}", data${this._parseKeys()});`);
                break;
            case 'char*':
                this._print(`BSON_APPEND_UTF8(${this._parseDepth()}, "${this._getKey()}", data${this._parseKeys()});`);
                break;
        }
    }

    _parseObject(data) {
        let oldDepth, newDepth;
        oldDepth = this._parseDepth();
        this.depth++;
        newDepth = this._parseDepth();

        this._print(`BSON_APPEND_DOCUMENT_BEGIN(${oldDepth}, "${this._getKey()}", ${newDepth});`);

        for (const key in data) {
            this.keys.push({ type: 'object', key });
            this._parse(data[key], key);
            this.keys.pop();
        }

        this._print(`bson_append_document_end(${oldDepth}, ${newDepth});`);
        this._print(`bson_destroy(${newDepth});`);

        this.depth--;

    }

    _parseArray(data) {
        let oldDepth, newDepth;
        oldDepth = this._parseDepth();
        this.depth++;
        newDepth = this._parseDepth();
        const counter = this._parseForDepth();
        this.forDepth++;

        this._print(`BSON_APPEND_ARRAY_BEGIN(${oldDepth}, "${this._getKey()}", ${newDepth});`);
        this._print(`for (int ${counter} = 0; ${counter} < (data${this._parseKeys()}_count); ${counter}++)`);
        this._print(`{`);

        this.indentation++;
        this.keys.push({ key: counter, type: 'array' });
        this._parse(data[0]);
        this.keys.pop();
        this.indentation--;

        this._print(`}`);
        this._print(`bson_append_array_end(${oldDepth}, ${newDepth});`);
        this._print(`bson_destroy(${newDepth});`);

        this.depth--;
        this.forDepth--;
    }

    _parse(data) {
        if (Array.isArray(data)) {
            this._parseArray(data);
        }
        else if (typeof data === 'object') {
            this._parseObject(data);
        }
        else {
            this._parsePrimitive(data);
        }
    }

    _firstParse(data) {
        for (const key in data) {
            this.keys.push({ key, type: 'object' });
            this._parse(data[key]);
            this.keys.pop();
        }
    }

    _print(str) {
        this.result += `${this._tabsFromIndentation()}${str}\n`;
    }

    generate() {
        this._print(`*sending = bson_new();`);
        this._print(`bson_t *children = (bson_t*)malloc(sizeof(bson_t) * ${this.maxDepth});`);
        this._firstParse(this.structure);
        return this.result;
    }

    constructor(structure) {
        this.structure = structure;
        this.maxDepth = this._getDepth();
        this.depth = 0;
        this.forDepth = 0;
        this.indentation = 0;
        this.keys = [];
        this.result = '';
    }

}

module.exports = BsonCodeGenerator;