/*
    RULES:
        THERE CANNOT BE ARRAYS DIRECTLY INSIDE OTHER ARRAYS
*/

class StructureInitializerCodeGenerator {

    _print(str) {
        this.result += `${str}\n`;
    }

    _getTypeName() {
        return (this.keys.length === 1 ? this.keys[0] : `${this.keys.slice(1).join('_')}_data`);
    }

    _getCountName() {
        const last = this.keys.length - 1;
        return `${this.keys[last]}_count`;
    }

    _parseKeys() {
        return this.keys.slice(1).reduce((accumulator, current) => accumulator += `.${current}`, '').replace('.', '->');
    }

    _parseKeysParent(count) {
        const last = this.keys.length - 1;
        return `${this.keys.slice(1, last).reduce((accumulator, current) => accumulator += `.${current}`, '')}.${count}`.replace('.', '->');
    }

    _parse(data) {
        for (const key in data) {
            if (Array.isArray(data[key])) {
                this.keys.push(key);
                const keys = this._parseKeys();
                const count = this._getCountName();
                const keysParent = this._parseKeysParent(count);
                const type = this._getTypeName();
                this._print(`data${keys} = (${type}*)malloc(sizeof(${type}) * ${data[key][1]});`);
                this._print(`data${keysParent} = 0;`);
                this._parse(data[key][0]);
                this.keys.pop();
            }
            else if (typeof data[key] === 'object') {
                this.keys.push(key);
                this._parse(data[key]);
                this.keys.pop();
            }
        }
    }

    generate() {
        this.keys.push({ key: 'data_t', type: 'object' });
        this._parse(this.structure);
        return this.result;
    }

    constructor(structure) {
        this.structure = structure;
        this.keys = [];
        this.result = '';
    }

}

module.exports = StructureInitializerCodeGenerator;