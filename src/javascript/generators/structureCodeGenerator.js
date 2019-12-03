/*
    RULES:
        THERE CANNOT BE ARRAYS DIRECTLY INSIDE OTHER ARRAYS
*/

class StructureCodeGenerator {

    _print(str) {
        this.structs[this.cursor] += `${this._tabsFromIndentation()}${str}\n`;
    }

    _tabsFromIndentation() {
        let res = '';
        for (let i = 0; i < this.indentation; i++) {
            res += '\t';
        }
        return res;
    }

    _getStructName() {
        return (this.prefixes.length === 1 ? this.prefixes[0] : `${this.prefixes.slice(1).join('_')}_data`);
    }

    _getCountName() {
        const last = this.prefixes.length - 1;
        return `${this.prefixes[last]}_count`;
    }

    _addStruct() {
        this.structs.splice(this.cursor + 1, 0, '');
    }

    _parse(data, name) {
        this._addStruct();
        this.prefixes.push(name);
        this.cursor++;
        this.indentation = 0;
        this._print(`typedef struct {`);
        this.indentation = 1;
        for (const key in data) {
            if (Array.isArray(data[key])) {
                this._parse(data[key][0], key);
                this._print(`${this._getStructName()} *${key};`);
                this._print(`int ${this._getCountName()};`);
                this.prefixes.pop();
            }
            else if (typeof data[key] === 'object') {
                this._parse(data[key], key);
                this._print(`${this._getStructName()} ${key};`);
                this.prefixes.pop();
            }
            else {
                this._print(`${data[key]} ${key};`);
            }
        }
        this.indentation = 0;
        this._print(`} ${this._getStructName()};`);
        this.indentation = 1;
        this.cursor--;
    }

    _mergeStructs() {
        let result = '';
        while (this.structs.length) {
            result += this.structs.pop() + '\n';
        }
        return result;
    }

    generate() {
        this._parse(this.structure, 'data_t');
        return this._mergeStructs();
    }

    constructor(structure) {
        this.structure = structure;
        this.prefixes = [];
        this.structs = [];
        this.cursor = -1;
        this.indentation = 0;
    }

}

module.exports = StructureCodeGenerator;