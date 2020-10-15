function log(message) {
    console.log(message)
}

var message2 = 'Hello World';

log(message2);

function doSomething() {
    let i = 0;
    for (i = 0; i<5; i++) {
        console.log(i);
    }
    console.log('Finally: ' + i);
}

doSomething();

let count: number;

count = 1;

let b: boolean;
let c: string;
let d: number[] = [1,2,3];
let e: any[] = [1, true, 'a'];

const ColorRed = 0;
const ColorGreen = 1;
const ColorBlue = 2;

enum Color { Red = 0, Green = 1, Blue = 2 };
let backgroundColor = Color.Red;

let message;
message = 'abc';

// type assertion
let endsWithC = (<string>message).endsWith('c');
let alternativeWay = (message as string).endsWith('c');


// let log = function(message) {
//     console.log(message);
// }
// rambda
let doLog1 = (message) => console.log(message);
let doLog2 = message => console.log(message);
let doLog3 = () => console.log("test");

// inline annotation

let drawPoint = (point: { x: number, y: number }) => {
    console.log(point.x);
    console.log(point.y);
}

drawPoint ({
    x: 5,
    y: 6
});

drawPoint ({
    x: 7,
    y: 8
})

// alternative to inline annotation
interface _Point {
    x: number, 
    y: number
}

let drawPoint2 = ( point:_Point ) => {
    console.log(point.x);
    console.log(point.y);
}

drawPoint2({x: 111, y: 222});

import { Point } from './point';

let point = new Point(3, 5);
point.draw();

let point2 = new Point(3);
point2.sety(333);
let x = point2.gety();
point2.sety(444);
point2.draw();

class Point2 {
    x: number;
    y: number;
}

let arr:Point2[] = [{x: 1, y: 2}, {x: 33, y: 44}];
console.log(arr[0].x);
console.log(arr[0].y);
console.log(arr[1].x);
console.log(arr[1].y);