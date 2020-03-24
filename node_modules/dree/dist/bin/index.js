#!/usr/bin/env node
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var yargs = require("yargs");
var path_1 = require("path");
var fs_1 = require("fs");
var index_1 = require("../lib/index");
function parseRegExp(patterns) {
    var result = [];
    if (patterns && patterns.length) {
        for (var _i = 0, patterns_1 = patterns; _i < patterns_1.length; _i++) {
            var pattern = patterns_1[_i];
            result.push(new RegExp(pattern.replace(/\\/g, '\\\\')));
        }
    }
    return result;
}
yargs
    .scriptName('dree')
    .command('parse <source>', 'Save the directory tree as a text file', function (yargs) {
    return yargs.positional('source', {
        describe: 'The path of the root of the directory tree',
        type: 'string'
    });
}, function (argv) {
    var args = argv;
    var options = {
        symbolicLinks: args.symbolicLinks,
        followLinks: args.followLinks,
        showHidden: args.showHidden,
        depth: args.depth,
        exclude: parseRegExp(args.exclude),
        extensions: args.extensions,
        skipErrors: args.skipErrors
    };
    var source = args.source;
    var dest = args.dest;
    var name = args.name;
    var show = args.show;
    var tree = index_1.parse(source, options);
    if (show) {
        console.log(tree);
    }
    fs_1.writeFileSync(path_1.join(dest, name + '.txt'), tree);
})
    .command('scan <source>', 'Save the directory tree as a json file', function (yargs) {
    return yargs.positional('source', {
        describe: 'The path of the root of the directory tree',
        type: 'string'
    });
}, function (argv) {
    var args = argv;
    var options = {
        stat: args.stat,
        normalize: args.normalize,
        symbolicLinks: args.symbolicLinks,
        followLinks: args.followLinks,
        sizeInBytes: args.sizeInBytes,
        size: args.size,
        hash: args.hash,
        hashAlgorithm: args.hashAlgorithm,
        hashEncoding: args.hashEncoding,
        showHidden: args.showHidden,
        depth: args.depth,
        exclude: parseRegExp(args.exclude),
        emptyDirectory: args.emptyDirectory,
        excludeEmptyDirectories: args.excludeEmptyDirectories,
        extensions: args.extensions,
        skipErrors: args.skipErrors
    };
    var source = args.source;
    var dest = args.dest;
    var name = args.name;
    var show = args.show;
    var tree = JSON.stringify(index_1.scan(source, options));
    if (show) {
        console.log(tree);
    }
    fs_1.writeFileSync(path_1.join(dest, name + '.json'), tree);
})
    .demandCommand(1, 'You must use either parse of scan command')
    .options({
    'dest': {
        alias: 'd',
        default: process.cwd(),
        defaultDescription: 'The path where the command was lunched',
        describe: 'The path of the output file destination',
        type: 'string'
    },
    'name': {
        alias: 'n',
        default: 'tree',
        describe: 'The output file name, without file extension',
        type: 'string'
    },
    'show': {
        alias: 's',
        default: false,
        describe: 'Wheter you want to print the result on the command line',
        type: 'boolean'
    },
    'stat': {
        default: false,
        describe: 'Wheter you want the fs.stat included in the json result',
        type: 'boolean',
        hidden: true
    },
    'normalize': {
        default: false,
        describe: 'Wheter you want to normalize the path in the json result',
        type: 'boolean',
        hidden: true
    },
    'symbolic-links': {
        default: true,
        describe: 'Wheter you want to consider symbolic links during the elaboration, could not work on windows',
        type: 'boolean',
        hidden: true
    },
    'follow-links': {
        default: false,
        describe: 'Wheter you want to follow symbolic links during the elaboration, could not work on windows',
        type: 'boolean',
        hidden: true
    },
    'size-in-bytes': {
        default: true,
        describe: 'Wheter you want to include the size in bytes in the json result',
        type: 'boolean',
        hidden: true
    },
    'size': {
        default: true,
        describe: 'Wheter you want to include the size in a proper unit in the json result',
        type: 'boolean',
        hidden: true
    },
    'hash': {
        default: true,
        describe: 'Wheter you want to include the hash in the json result',
        type: 'boolean',
        hidden: true
    },
    'hash-algorithm': {
        default: 'md5',
        describe: 'The hash algorithm that you want to use for the hash in the json result',
        type: 'string',
        choices: ['md5', 'sha1'],
        hidden: true
    },
    'hash-encoding': {
        default: 'hex',
        describe: 'The hash encoding that you want to use for the hash in the json result',
        type: 'string',
        choiches: ['hex', 'base64', 'latin1'],
        hidden: true
    },
    'show-hidden': {
        default: true,
        describe: 'Wheter you want to consider hidden files during the elaboration',
        type: 'boolean',
        hidden: true
    },
    'depth': {
        default: undefined,
        describe: 'The max depth wich could be reached during the elaboration from the given folder',
        type: 'number',
        hidden: true
    },
    'exclude': {
        describe: 'An array of regex whose all matched path will not be considered during the elaboration',
        type: 'array',
        hidden: true
    },
    'extensions': {
        describe: 'An array of strings containing all the extensions wich will be considered',
        type: 'array',
        hidden: true
    },
    'empty-directory': {
        default: false,
        describe: 'Wheter you want to include the property isEmpty in the result',
        type: 'boolean',
        hidden: true
    },
    'exclude-empty-directories': {
        default: false,
        describe: 'Wheter you want to exclude all the empty directories from the result, even if they contains nodes excluded by other options',
        type: 'boolean',
        hidden: true
    },
    'skip-errors': {
        default: true,
        describe: 'Wheter you want to skip folders and files wich give errors during the execution',
        type: 'boolean',
        hidden: true
    },
    'options': {
        alias: 'o',
        describe: 'A path to a json config file. If an option is both on the file and in the command, the command one will be considered',
        config: true
    }
})
    .showHidden('all-options')
    .epilogue('For more information, find our manual at https://github.com/euberdeveloper/dree#readme')
    .argv;
//# sourceMappingURL=index.js.map