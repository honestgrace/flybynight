
// Class Declaration  -> this is hoisted.
class Foo {

}

// Class Expression   -> this is not hoisted.
const Bar = class {

}

const _hiddenProperty = Symbol();
const hiddenDraw = Symbol();
const _radius = new WeakMap();
const _move = new WeakMap();
class Circle {
    constructor(radius) {
        this.radius = radius;

        // private property
        // this[_hiddenProperty] = radius;
        _radius.set(this, radius);

        // intance level function
        this.move = function() {  
        }

        // private method
        _move.set(this, ()=> {     // arrow operator is required to make this works!!!
            console.log('move', this.radius);
        });
    }

    // getter
    get Radius() {
        return radius;
    }
    set Radius(value) {
        if (value <= 0) throw new Error('invalid radius');
        radius = value;
    }

    // instance method
    // prototype level function
    draw() {                      
        console.log('draw');
        // console.log('hidden: ' + this[_hiddenProperty]);
        console.log('_radius: ' + _radius.get(this));
        _move.get(this)();
    }

    [hiddenDraw]() {
        console.log('hidden draw');
    }

    // static method
    static parse(str) {
        const radius = JSON.parse(str).radius;
        return new Circle(radius);
    }
}

const c = new Circle(1);
const c2 = Circle.parse('{ "radius": 1}');
console.log(c2);


class Shape {
    constructor(radius) {
        this.radius = radius;
    }
    exist() {
        console.log('I am here');
    }
}

class Square extends Shape {
    constructor(radius, color) {
        super(radius);   // call the constructer of parent first
        this.color = color;
    }
    draw() {
        console.log("draw " + this.radius + "," + this.color);
    }
    exist() {
        super.exist();
        console.log('Square is here');
    }
}

let sq = new Square(5, 'red');
sq.exist();
sq.draw();




const _items = new WeakMap();
class Stack {
    constructor() {
        _items.set(this, []);
    }

    get count() {
        return _items.get(this).length;
    }

    peek() {
        const items = _items.get(this);
        if (items.length == 0) 
            throw new Error('Stack is empty');

        return items[items.length - 1];
    }

    push(item) {
        _items.get(this).push(item);
    }

    pop() {
        const items = _items.get(this);
        if (items.length == 0) 
            throw new Error('Stack is empty');
        
        return items.pop();
    }
}

const stack = new Stack();
stack.push('a');
stack.count;
stack.pop();
