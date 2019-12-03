function addIndentation(row, block) {
    const initial = row.split('//')[0];
    return block
                .split('\n')
                .map(row => initial + row)
                .join('\n');
}

module.exports = addIndentation;