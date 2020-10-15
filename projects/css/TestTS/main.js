"use strict";
exports.__esModule = true;
function log(message) {
    console.log(message);
}
var message2 = 'Hello World';
log(message2);
function doSomething() {
    var i = 0;
    for (i = 0; i < 5; i++) {
        console.log(i);
    }
    console.log('Finally: ' + i);
}
doSomething();
var count;
count = 1;
var b;
var c;
var d = [1, 2, 3];
var e = [1, true, 'a'];
var ColorRed = 0;
var ColorGreen = 1;
var ColorBlue = 2;
var Color;
(function (Color) {
    Color[Color["Red"] = 0] = "Red";
    Color[Color["Green"] = 1] = "Green";
    Color[Color["Blue"] = 2] = "Blue";
})(Color || (Color = {}));
;
var backgroundColor = Color.Red;
var message;
message = 'abc';
// type assertion
var endsWithC = message.endsWith('c');
var alternativeWay = message.endsWith('c');
// let log = function(message) {
//     console.log(message);
// }
// rambda
var doLog1 = function (message) { return console.log(message); };
var doLog2 = function (message) { return console.log(message); };
var doLog3 = function () { return console.log("test"); };
// inline annotation
var drawPoint = function (point) {
    console.log(point.x);
    console.log(point.y);
};
drawPoint({
    x: 5,
    y: 6
});
drawPoint({
    x: 7,
    y: 8
});
var drawPoint2 = function (point) {
    console.log(point.x);
    console.log(point.y);
};
drawPoint2({ x: 111, y: 222 });
var point_1 = require("./point");
var point = new point_1.Point(3, 5);
point.draw();
var point2 = new point_1.Point(3);
point2.sety(333);
var x = point2.gety();
point2.sety(444);
point2.draw();
