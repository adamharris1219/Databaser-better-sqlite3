'use strict';
const fs = require('fs-extra');
const path = require('path');
const os = require('os');
const chai = require('chai');

const tempDir = path.join(__dirname, '..', 'temp');
let dbId = 0;

global.expect = chai.expect;
global.util = {
	current: () => path.join(tempDir, `${dbId}.db`),
	next: () => (++dbId, global.util.current()),
	describeUnix: os.platform().startsWith('win') ? describe.skip : describe,
};

before(function () {
	fs.removeSync(tempDir);
	fs.ensureDirSync(tempDir);
});

after(function () {
	fs.removeSync(tempDir);
});
