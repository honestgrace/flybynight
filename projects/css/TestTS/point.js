"use strict";
// Cohesion
// class Point {
//     private x: number;
//     private y: number;
exports.__esModule = true;
//     constructor(x: number, y?: number) {
//         this.x = x;
//         this.y = y;
//     }
//     draw() {
//         console.log(this.x);
//         console.log(this.y);
//     }
//     getDistance(another: Point) {
//     }
// }
var Point = /** @class */ (function () {
    function Point(_x, _y) {
        this._x = _x;
        this._y = _y;
    }
    Point.prototype.draw = function () {
        console.log(this._x);
        console.log(this._y);
    };
    Point.prototype.getDistance = function (another) {
    };
    // getter
    Point.prototype.gety = function () {
        return this._y;
    };
    // setter
    Point.prototype.sety = function (value) {
        if (value < 0)
            throw new Error('value cannot be less than 0');
        this._y = value;
    };
    return Point;
}());
exports.Point = Point;
