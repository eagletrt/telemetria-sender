function getCodes(generators, structure) {
    return Object.keys(generators)
        .map(key => ({
            comment: generators[key].comment,
            code: (new generators[key].generator(structure)).generate()
        }));
}

module.exports = getCodes;