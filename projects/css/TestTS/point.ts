// Cohesion
// class Point {
//     private x: number;
//     private y: number;

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

export class Point {
    constructor(public _x: number, private _y?: number) {}
    
    draw() {
        console.log(this._x);
        console.log(this._y);
    }
    getDistance(another: Point) {
    }

    // getter
    gety() {
        return this._y;
    }

    // setter
    sety(value) {
        if (value < 0)
            throw new Error('value cannot be less than 0');

        this._y = value;
    }
}