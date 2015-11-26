var winston = require('winston');
var util = require('util');
var wce = require('./index');
var logger = new (winston.Logger)({});
logger.add(winston.transports.Console, {
  prettyPrint: true,
  colorize: true
});

var extractors = ['read-art', 'node-readability'];
var options = {};
var WCE = new wce(extractors, options);

try {
  WCE.extract('http://www.nytimes.com/aponline/2015/11/11/world/europe/ap-eu-europe-migrants-the-latest.html')
    .on('success', function (result, errors) {
      logger.log('info', result);
      if (errors && errors.length !== 0) {
        logger.log('warn', 'Extraction was successful, but there were some errors: %s', util.inspect(errors));
      }
    })
    .on('error', function (errors) {
      logger.log('error', 'Extraction failed with the following error(s): %s', util.inspect(errors));
    });
} catch (error) {
  logger.log('error', util.inspect(error));
}
