var nodemailer = require('nodemailer');
const username = 'file.distiller@outlook.com';
const password = 'dacoolestprojectever!123';
const password2 = 'jgdtgndhbvsvlpex'
var sender = nodemailer.createTransport(
    {service: 'Outlook365', auth: {user: username, pass: password}});

function sendMail(to_address, file_name, callback) {
  console.log(`sending to: ${to_address}`);
  const mail = {
    from: username,
    to: to_address,
    subject: 'distilled Office file',
    text: 'Here is your distilled office file',
    attachments: [{
      filename: file_name,
      path: __dirname + `/distilled/${file_name}`,
      cid: file_name
    }]
  };
  sender.sendMail(mail, callback);
}

module.exports = {sendMail}