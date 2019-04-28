/*
 * Lambda Function - MM - Final Version - 1/03/19
 * Back end to the Alexa Skill Kit
 */


const http = require('http');
const Alexa = require('ask-sdk');

/*
 * LaunchHandler object - skill invocation
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const LaunchRequestHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'LaunchRequest';
  },
  handle(handlerInput) {

    return handlerInput.responseBuilder
      .speak("The Gari line is open")
      .reprompt("What would you like to ask him?")
      .getResponse();
  },
};

/*
 * AliveHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const AliveHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'Alive';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 1
   */
  async handle(handlerInput) {
    await httpGet('1');
    
    return handlerInput.responseBuilder
            .speak(' Gari is ready and listening at your command ')
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * ForwardHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const ForwardHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'Forward';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 2
   */
  async handle(handlerInput) {
    await httpGet('2');

    return handlerInput.responseBuilder
            .speak(" Gari is moving forward, he is having fun ")
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * BackwardHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const BackwardHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'Backward';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 3
   */
  async handle(handlerInput) {
    await httpGet('3');

    return handlerInput.responseBuilder
            .speak(" Gari is moving backwards, watch out!  ")
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * TurnLeftHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const TurnLeftHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'TurnLeft';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 4
   */
  async handle(handlerInput) {
    await httpGet('4');

    return handlerInput.responseBuilder
            .speak(" Gari is making a left turn! ")
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * TurnRightHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const TurnRightHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'TurnRight';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 5
   */
  async handle(handlerInput) {
    await httpGet('5');

    return handlerInput.responseBuilder
            .speak(" Gari is making a right turn! ")
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * SpinLeftHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const SpinLeftHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'SpinLeft';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 6
   */
  async handle(handlerInput) {
    await httpGet('6');

    return handlerInput.responseBuilder
            .speak(" Gari is spinning left, look out everyone! ")
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * SpinRightHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const SpinRightHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'SpinRight';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 7
   */
  async handle(handlerInput) {
    await httpGet('7');

    return handlerInput.responseBuilder
            .speak(" Gari is spinning right, here he goes! ")
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * LookAroundHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const LookAroundHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'LookAround';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 8
   */
  async handle(handlerInput) {
    await httpGet('8');

    return handlerInput.responseBuilder
            .speak(" Gari is looking around. He can see you all watching him! ")
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * GreetingHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const GreetingHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'Greeting';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 9
   */
  async handle(handlerInput) {
    await httpGet('9');

    return handlerInput.responseBuilder
            .speak(" Gari says hello. I think he likes you! ")
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * FeelingHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const FeelingHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'Feeling';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 10
   */
  async handle(handlerInput) {
    await httpGet10('10');

    return handlerInput.responseBuilder
            .speak(" Gari is feeling shy! Everyone is watching him! ")
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * JokeHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const JokeHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'Joke';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 11
   */
  async handle(handlerInput) {
    await httpGet11('11');

    return handlerInput.responseBuilder
            .speak(" Hahahaha Gari enjoyed that one! That's made him happy! ")
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * ExerciseHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const ExerciseHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'Exercise';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 12
   */
  async handle(handlerInput) {
    await httpGet12('12');

    return handlerInput.responseBuilder
            .speak(" Go on Gari! He loves a good workout! ")
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * BallHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const BallHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'Ball';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 13
   */
  async handle(handlerInput) {
    await httpGet13('13');

    return handlerInput.responseBuilder
            .speak(" Gari love football! Great kick Gari ! ")
            .withShouldEndSession(true)
            .getResponse();
  },
};

/*
 * RaveHandler object - custom intent
 * Contains two inline functions: canHandle and handle.
 * canHandle tests for support confirmation
 * handle calls the response builder to build the Alexa data response out
 */
const RaveHandler = {
  canHandle(handlerInput) {
    const request = handlerInput.requestEnvelope.request;
    return request.type === 'IntentRequest'
      && request.intent.name === 'Rave';
  },

  /*
   * Asynchronous, waiting for the http request to be complete before returning
   * Calls function with Id 14
   */
  async handle(handlerInput) {
    await httpGet('14');

    return handlerInput.responseBuilder
            .addAudioPlayerPlayDirective('REPLACE_ALL', 'https://www.globebyte.com/gari.mp3', 'garirave', 0, null)
            .withShouldEndSession(true)
            .getResponse();
  },
};


const skillBuilder = Alexa.SkillBuilders.standard();

/*
 * Exporting handler objects for node
 */
exports.handler = skillBuilder
  .addRequestHandlers(
    LaunchRequestHandler,
    AliveHandler,
    ForwardHandler,
    BackwardHandler,
    TurnLeftHandler,
    TurnRightHandler,
    SpinLeftHandler,
    SpinRightHandler,
    LookAroundHandler,
    GreetingHandler,
    FeelingHandler,
    JokeHandler,
    ExerciseHandler,
    BallHandler,
    RaveHandler
  )
  .lambda();

/*
 * Purpose: Makes HTTP GET Request to server with commandId
 * Parameters:
 *    commandId - From specific skill intent
 * Return:
 *    nothing
 */
function httpGet(commandId) {
  return new Promise((resolve, reject) => {
    const options = {
        host: 'gari-gizmo-server.herokuapp.com',
        port: process.env.PORT || 80,
        path: '/restapi/' + commandId + '/0',
        method: 'GET'
    };
    
    const req = http.request(options, (res) => {
      resolve('Success');
    });
     
    req.on('error', (e) => {
      reject(e.message);
    });


    req.write('');
    req.end();
  });
};

