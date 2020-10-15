function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}
async function demo() {
    await sleep(5000);
    console.log('Demo finished...');
}

function getUser(id) {
    console.log(`getUser called for ${id}`);
    return new Promise((resolve, reject) => {
        setTimeout(()=> {
            console.log('Reading a user from a database...');
            resolve({ id: id, name: "test"});
        }, 2000);
    })
}

function getRepositories(username) {
    console.log(`getRepositories called for ${username}`);
    return new Promise((resolve, reject) => {
        setTimeout(()=> {
            console.log('Reading repositories...');
            resolve(['redo1', 'repo2', 'repo3']);
        }, 2000);
    })
}

function getCommits(repo) {
    console.log(`getCommits called for ${repo}`);
    return new Promise((resolve, reject) => {
        setTimeout(()=> {
            resolve(['#1234', '#34432']);
        }, 2000);
    })
}

// getUser(1)
// .then(user => getRepositories(user.name))
// .then(repos => { 
//     console.log('Repos:', repos);
//     getCommits(repos[0]);
// })
// .then(commits => console.log('Commits:', commits));

async function displayCommits() {
    try {
        const user = await getUser(1);
        const repos = await getRepositories(user);
        const commits = await getCommits(repos[0]);
        console.log('Commits:', commits)
    } catch (err) {
        console.log ('Error', err);
    }
}

console.log('Before');
displayCommits();
console.log('After');
