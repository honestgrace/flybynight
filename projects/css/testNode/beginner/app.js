const Logger = require('./logger');
const logger = new Logger();

const path = require('path');
const os = require('os');
const fs = require('fs');

logger.log('Kim');

var temp = path.parse(__filename);

logger.log (temp);
logger.log (temp.name);
var temp = os.totalmem();
var temp2 = os.freemem();

logger.log (temp);
logger.log (temp2)

// Template string
// ES6 /ES2015: ECMAScript 6

logger.log (`Total Memory: ${temp}`)
logger.log (`Free Memory: ${temp2}`)

const files = fs.readdirSync('./');
logger.log (files);

fs.readdir('./', function(err, files) { 
   if (err) {
       console.log('Error', err);
   } else {
       logger.log(`Results ${files}`);
   }
});

// Register listener
logger.on('messageLogged', (arg) => {
    console.log('Arg:', arg);
})

logger.log('message');

