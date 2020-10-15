
// Factory function
function createCircle(radius) {
    return {
        radius,
        draw: function() {
            console.log('draw');
        }
    }
}

// Constructor function
function Circle(radius) {
    this.radius = radius;
    this.draw = function() {
        console.log('draw by Circle');
    }
}

let y = createCircle();
y.draw();
let z = new Circle(3);
z.draw();
let x = {};

const Circle1 = new Function('radius', `
this.radius = radius;
this.draw = function() {
    console.log('draw by Circle1');
}
`);

(new Circle1()).draw();

let input = {foo: 'bar'};
Circle.call(input, 1);
input.draw();

/*
Value type:
    Number
    String
    Boolean
    Symbol
    undefined
    null

Reference type:
    Function
    Object
    Array
*/


// Constructor function
function NewCircle(radius) {
    this.radius = radius;
    let defaultLocation = { x: 0, y: 0};
    this.getDefaulterLocation = function() {
        return defaultLocation;
    }
    this.draw = function() {
        computeOptimumLocation();
        console.log('draw by NewCircle');
    }
    let computeOptimumLocation = function() {
        // ...
    }
    Object.defineProperty(this, 'DefaultLocation', {
        get: function() {
            return defaultLocation;
        },
        set: function(value) {
            if (!value.x || !value.y)
                throw new Error('Invalid location');
            defaultLocation = value;
        }
    })
}

const cc = new NewCircle(10);
cc.draw();
console.log(cc.DefaultLocation);
cc.DefaultLocation = {x: 5, y: 10};
console.log(cc.DefaultLocation);

const sw = new Stopwatch()
sw.duration;
sw.start();
sw.start();
sw.stop();
sw.stop();
sw.duration;
sw.start();
sw.reset();
sw.duration;

function Stopwatch() {
    let startDate = new Date();
    let endDate = new Date();
    let running = false;
    this.stop = function() {
        if (!running) return undefined;
        endDate = new Date();
        running = false;
        console.log('stopped');
    }
    this.reset = function() {
        startDate = new Date();
    }
    this.start = function() {
        if (running) return undefined;
        running = true;
        console.log('started');
        this.reset();
    }
    Object.defineProperty(this, 'duration', {
        get: function() {
            if (running) {
                return (new Date()).getTime() - startDate.getTime();
            }
            return endDate.getTime() - startDate.getTime();
        }
    })
}



