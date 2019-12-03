const bsonCodeGenerator = require('./bsonCodeGenerator');
const structureCodeGenerator = require('./structureCodeGenerator');
const structureInitializerCodeGenerator = require('./structureInitializerCodeGenerator');
const structureDeallocatorCodeGenerator = require('./structureDeallocatorCodeGenerator');

module.exports = {
    bsonCodeGenerator: { comment: '{{GENERATE_BSON_CODE}}', generator: bsonCodeGenerator },
    structureCodeGenerator: { comment: '{{GENERATE_STRUCTURE_CODE}}', generator: structureCodeGenerator },
    structureInitializerCodeGenerator: { comment: '{{GENERATE_STRUCTURE_INITIALIZER_CODE}}', generator: structureInitializerCodeGenerator },
    structureDeallocatorCodeGenerator: { comment: '{{GENERATE_STRUCTURE_DEALLOCATOR_CODE}}', generator: structureDeallocatorCodeGenerator }
};