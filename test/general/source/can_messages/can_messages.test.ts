import * as dree from 'dree';

export default function () {
    describe('Test if can messages are correctly parsed, sent and saved', function() {

    
    });
}

function testFolder(name: string, path: string): void {
    describe(`Test ${name} messages group`, function() {

    
    });

    const folders: dree.Dree[] = [];
    const messageFolders: dree.Dree[] = [];

    dree.scan(path, {}, () => {}, folder => {
        if (/\.message$/.test(folder.name)) {
            messageFolders.push(folder);
        }
        else {
            folders.push(folder);
        }
    });


}