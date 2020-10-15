class Circle {
    constructor(foo) {
	this.foo = 'bar'
    }

    debug() {
	const success = true;
	const p = new Promise((resolve, reject) => {
	    setTimeout(()=>{
                console.log('setTimeout foo', this.foo);
		if (success) {
		  console.log('succeeded from promise object');
		  resolve(1);
	       } else {
	          console.log('failed from promise object');
    	          reject(new Error('error happened'));
	       }

	    }, 2000)
	});

	p
	  .then(r => {
	          console.log('Result', r);
	          console.log('then foo', this.foo);
	        })
	  .catch(e => {
	          console.log('Error', e.message);
	          console.log('catch foo', this.foo);
	        });
    }
}

let c = new Circle();
c.debug();
c = null;