const p = new Promise((resolve, reject) => {
    setTimeout(() => {
        resolve(1);
        //reject(new Error('message'));
}, 2000);
});

p
  .then(result => console.log('Result', result))
  .catch(err => console.log('Error', err.message));

function test() {
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            console.log(2);
            resolve(2);
            //reject(new Error('message'));
    }, 2000);
    });
}

function test2(arg) {
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            console.log(arg * 2);
            resolve(arg * 2);
            //reject(new Error('message'));
    }, 2000);
    });
}

async function AsycTest()
{
    const arg = await test();
    const result2 = await test2(arg);
}
AsycTest();




