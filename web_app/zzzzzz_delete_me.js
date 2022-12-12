// // const { spawn } = require('child_process');

// // const child = spawn('sleep', ['3']);

// // child.on('exit', function (code, signal) {
// //   console.log('child process terminated with code ' + code);
// //   console.log('done');
// // });


// var nodemailer = require("nodemailer");
// const username = 'file.distiller@gmail.com';
// const password2 = 'dacoolestprojectever!123';
// const password = 'jgdtgndhbvsvlpex'
// var sender = nodemailer.createTransport({
// service: 'gmail',
// auth: {
// 	user: username,
// 	pass: password 
// }
// });

// var mail = {
// from: username,
// to: username,
// subject: "Sending Email using Node.js",
// text: "That was easy!",
// attachments: [
//   {
//       filename: 'document0.docx',
//       path: __dirname + '/uploads/document0.docx',
//       cid: 'document0.docx'
//   }
// ]
// };

// sender.sendMail(mail, function(error, info) {
// if (error) {
// 	console.log(error);
// } else {
// 	console.log("Email sent successfully: "
// 				+ info.response);
// }
// });

const {sendMail} = require('./email_handler');

const callback = (error, info) => {
  if (error) {
    console.log(error);
  } else {
    console.log(info);
    console.log('success');
  }
}


sendMail("file.distiller@gmail.com", `document${0}.docx`, callback);