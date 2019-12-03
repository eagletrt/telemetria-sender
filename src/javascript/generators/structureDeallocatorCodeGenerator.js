/*
    RULES:
        THERE CANNOT BE ARRAYS DIRECTLY INSIDE OTHER ARRAYS
*/

class StructureDeallocatorCodeGenerator {

    _print(str) {
        this.result += `${str}\n`;
    }

    _parseKeys() {
        return this.keys.reduce((accumulator, current) => accumulator += `.${current}`, '').replace('.', '->');
    }

    _parse(data) {
        for (const key in data) {
            if (Array.isArray(data[key])) {
                this.keys.push(key);
                this._parse(data[key][0]);
                this._print(`free(data${this._parseKeys()});`);
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
        this._parse(this.structure);
        this._print(`free(data);`);
        return this.result;
    }

    constructor(structure) {
        this.structure = structure;
        this.keys = [];
        this.result = '';
    }

}

module.exports = StructureDeallocatorCodeGenerator;