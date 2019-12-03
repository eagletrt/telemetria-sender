const dree = require('dree');
const path = require('path');
const fs = require('fs');
const utils = require('./utils');
const generators = require('./generators');

const STRUCTURE_PATH = path.join(__dirname, '../', 'structure.json');
const structure = require(STRUCTURE_PATH);

const ROOT_PATH = path.join(process.cwd());

const codes = utils.getCodes(generators, structure);

dree.scan(ROOT_PATH, { exclude: [ /javascript/, /node_modules/ ] }, (file, _stat) => {
    if (file.name.indexOf('.template') !== -1) {
        const template = fs.readFileSync(file.path, 'utf-8');
        const generated = utils.parseTemplate(template, codes);
        const generatedPath = file.path.replace('.template', '');
        fs.writeFileSync(generatedPath, generated);
    }
});