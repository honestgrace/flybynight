function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}
async function demo() {
    await sleep(5000);
    console.log('Demo finished...');
}

/*
function getUser(id, callbacks) {
    setTimeout(()=> {
        console.log('Reading a user from a database...');
        callbacks({ id: id, name: "test"});
    }, 2000);
}

function getRepositories(username, callbacks) {
    setTimeout(()=> {
        console.log('Reading repositories...');
        callbacks(['redo1', 'repo2', 'repo3']);
    }, 2000);
}

console.log('Before');
getUser(1, function(user){
    console.log('User', user);
    getRepositories(user.username, function(repositories) {
        console.log('Repositories', repositories);
    });
});
console.log('After');
*/

// callbacks
// promises
// async / await

/* alternative way to solve aync hell */

function getUser(id, callbacks) {
    setTimeout(()=> {
        console.log('Reading a user from a database...');
        callbacks({ id: id, name: "test"});
    }, 2000);
}

function getRepositories(username, callbacks) {
    setTimeout(()=> {
        console.log('Reading repositories...');
        callbacks(['redo1', 'repo2', 'repo3']);
    }, 2000);
}

function displayUser(user) {
    console.log('User', user);
    getRepositories(user.username, displayRepositories);
}

function displayRepositories(repositories) {
    console.log('Repositories', repositories);
    getCommits(repositories, getCommits);
}

function getCommits(commits) {
    console.log(commits);
}

console.log('Before');
getUser(1, displayUser);
console.log('After');