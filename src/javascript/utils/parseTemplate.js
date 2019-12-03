function addInitialIndentation(row, block) {
    const initial = row.split('//')[0];
    return block
        .split('\n')
        .map(row => initial + row)
        .join('\n');
}

function parseTemplate(template, codes) {
    return template.split('\n').map(row => codes
        .filter(({ comment }) => row.indexOf(comment) !== -1)
        .map(({ code }) => addInitialIndentation(row, code)).join('') || row
    ).join('\n');
}

module.exports = parseTemplate;