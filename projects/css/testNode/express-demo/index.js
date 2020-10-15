const startupDebugger = require('debug')('app:startup');  // set DEBUG=app:startup 
                                                          // set Debug=app:startup,app:db
                                                          // set Debug=app:*
const dbDebugger = require('debug')('app:db');

const config = require('config');
const morgan = require('morgan');
const helmet = require('helmet');
const Joi = require('joi');
const express = require('express');
const app = express();
const logger = require('./middleware/logger');
const courses = require('./routes/courses');
const home = require('./routes/home');

console.log(`NODE_ENV: ${process.env.NODE_ENV}`);
console.log(`app: ${app.get('env')}`);

app.set('view engine', 'pug');
app.set('views', './views'); // default

app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(express.static('public'));
app.use(helmet());
app.use('/api/courses', courses);
app.use('/', home);

// Configuration
console.log('Application Name: ' + config.get('name'));
console.log('Mail Server: ' + config.get('mail.host'));
try {
    config.get('mail.password')
    console.log('Mail Password: ' + config.get('mail.password'));
} catch {
    console.log('try again with executing set app_password=123');
}

console.log('Mail Password: ' + config.get('mail.password'));

// NOTE: you can set production to override the default value (development).
// set NODE_ENV=production
//
if (app.get('env') === 'development') {
    startupDebugger('Morgan enabled...');
    app.use(morgan('tiny'));
}

app.use(logger);
app.use(function(req, res, next) {
    dbDebugger('Authenticating...');
    next();
})

// PORT
const port = process.env.PORT || 3000;
app.listen(port, () => {
    console.log(`Listening on port ${port}...`);
})