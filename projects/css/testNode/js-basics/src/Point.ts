// tsc .\src\index.ts --outdir .\
export interface PointValue {
	x: number;
	y: number;
};

export class Point {
	constructor(private value: PointValue) {
	}

	draw() {
		console.log(this.value.x);
		console.log(this.value.y);
    };
    
    getDistance(another: Point) {
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