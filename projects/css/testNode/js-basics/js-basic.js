let person = { 
    name: 'Mosh',
    age: 30
}; // object literal

console.log(person);

// Dot Notation
person.name = 'John';

console.log(person.name);

// Bracket Notation
let selection = 'name';
person[selection] = 'Mary';

console.log(person.name);

let selectedColors = [
    'red', 'blue'
];  // array literal
selectedColors[2] = 1;

console.log(selectedColors.length);
console.log(selectedColors[1]);

// // const Test = require('./utility');
// // const test = new Test();
// const tests = require('./utility');
// console.log (tests);
// var test = tests['test'];
// console.log (test.Add(3, 5));
// console.log (tests.testFunction(3, 5));

// for (let a in tests) {
//     console.log (a);
// }

// for (let index in tests.test.GetData()) {
//     console.log (tests.test.GetData()[index]);
// }

// for (let a of tests.test.GetData()) {
//     console.log (a);
// }


// var array = [100000]
// for (let limit of array) console.log(`input data ${limit} : result ${showPrimes(limit)}`);

// function showPrimes(limit) {
//     for (let number = 1; number <= limit; number++) {

//         let isPrime = true;
//         for (let factor = 2; factor <= number/2; factor++) {
//             if (number % factor === 0) {
//                 isPrime = false;
//                 break;
//             }
//         }

//         if (isPrime) console.log(number);
//     }    
// }

// factory function - camel notation
function createCircle(radius) {
    return {
        radius,
        draw() {
            console.log ('draw');
        }
    }
}

const circle1 = createCircle(1);
circle1.draw();

// constructor function - pascal notation
function Circle(radius) {
    this.radius = radius;  // this is empty object
    this.draw = function() {
        console.log('draw');
    }
}

const circle2 = new Circle(1);
// => circle2 = Circle.call({}, 1);    {} is mapped to "this"
circle2.draw();

const circle = {
    radius: 1
}
circle.color = 'yellow';
circle.draw = function() {}
delete circle.color;

console.log(circle);

let z = {};
// => let z = new Object();

new String();  // ''
new Boolean(); // true, false
new Number(); // 1, 2, 3

let x = { name: 'x', value: 10 };
console.log (x);
let y = x;
y.value = 20;
console.log (x);
console.log (y);

for (let key of Object.keys(x))
    console.log(key);

for (let entry of Object.entries(x))
    console.log(entry);

if ('name' in x) console.log('name exists in object x');

const another = {};
for (let key in x)
    another[key] = x[key];

console.log(another);

const another2 = Object.assign({}, x);

const another3 = { ...x };
console.log(another3);

