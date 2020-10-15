walk();

// Hoisting => Move all function declaratio to top

// Function Declaration
function walk() {
    console.log('walk');
}

// Anonymous Function Expression
const run = function(){
    console.log('run');
};
run();

let move = run;
move();

function sum () {
    let total = 0;
    for (let item of arguments) 
        total += item;
    return total;
}

console.log(sum(1, 2));
console.log(sum(1));
console.log(sum());
console.log(sum(1, 2, 3, 4));

// Rest operator   => placed at the end
function sum2 (...args) {
    return args.reduce((a,b) => a + b);
}

console.log(sum2(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));

function interest(principals, rate = 3.5, years) {
    return principals * rate / 100 * years;
}

console.log(interest(10000, undefined, 1));

function interest2(principals, rate, years) {
    rate = rate || 3.5;
    years = years || 1;
    return principals * rate / 100 * years;
}

console.log(interest2(10000, undefined, undefined));
console.log(interest2(10000));

// getter and setters

const person = {
    firstName: 'Mosh',
    lastName: 'Hamedani',
    get fullName() {
        return `${person.firstName} ${person.lastName}`;
    },
    set fullName(value) {
        if (typeof value !== 'string') 
            throw new Error('Value is not a string.');
        const parts = value.split(' ');
        if (parts.length !== 2)
            throw new Error('Enter a first and last name');
        this.firstName = parts[0];
        this.lastName = parts[1];
    }
};

try {
    //person.fullName = null;
    //person.fullName = '';
    person.fullName = 'Jeong Kim';
} 
catch (e) {
    alert(e);
}

console.log(person.fullName);

const video = {
    title: 'a',
    tags: ['a', 'b', 'c'],
    showTags() {
        this.tags.forEach(tag => console.log(tag))
    }
}
video.showTags();


const video2 = {
    title: 'a',
    tags: ['a', 'b', 'c'],
    showTags() {
        this.tags.forEach(function(tag) { 
            console.log(this.title + ':' + tag);
        }, this)
    }
}
video2.showTags();

function playVideo(a, b) {
    console.log(this);
    console.log(a, b);
}


// meaning of this
// 1. inside of method => obj itself
// 2. inside of regular function => global object such as Window
// 3. inside of function after arrow (=>) obj itself 

playVideo();  // ==> will show Window with this
playVideo.call({foo: 'bar'}, 1, 2);  // ==> will show foo/bar object with this
playVideo.apply({foo: 'bar'}, [1, 2]);  // ==> will show foo/bar object with this
playVideo.bind({foo: 'bar'})(1,2);     // create a new function binding the given object for this

const video3 = {
    title: 'a',
    tags: ['a', 'b', 'c'],
    showTags() {
        this.tags.forEach(function(tag) { 
            console.log(this.title + ':' + tag);
        }.bind(this));
    }
}
video3.showTags();

const video4 = {
    title: 'a',
    tags: ['a', 'b', 'c'],
    showTags() {
        this.tags.forEach(tag => { 
            console.log(this.title + ':' + tag);
        });
    }
}
video4.showTags();

console.log (sum5(1, 2, 3, 4));
console.log (sum5([1, 2, 3]));
function sum5(...items) {
    if (items.length === 1 && Array.isArray(items[0]))
        items = [...items[0]];
    
    return items.reduce((a, b) => a + b);
}

const circle = {
    radius: 2,
    get area() {
        return this.radius ** 2 * Math.PI;
    }
}
console.log(circle.area);