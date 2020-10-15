;
export class Point {
    constructor(value) {
        this.value = value;
    }
    draw() {
        console.log(this.value.x);
        console.log(this.value.y);
    }
    ;
    getDistance(another) {
        console.log(another.value.x - this.value.x);
        console.log(another.value.y - this.value.y);
    }
    get x() {
        return this.value.x;
    }
    set x(value) {
        this.value.x = value;
    }
}
