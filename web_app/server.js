const express = require('express');
const multer = require('multer');
const fs = require('fs');
const {spawn} = require('child_process');
const {sendMail} = require('./email_handler');
const app = express();
const port = process.env.PORT || 5000;

const upload = multer({dest: 'uploads/'});
let i = 0;

app.post('/api/upload', upload.single('file'), (req, res) => {
  const new_file_name = `uploads/document${i}.docx`;
  const distilled_file_name = `document${i}.docx`;
  i++;
  fs.rename(req.file.path, new_file_name, function(err) {
    if (err) {
      console.log(err);
    }
  });
  // TODO: replace with: let child = spawn('<distil file executable>',
  // [new_file_name]);
  let child = spawn('sleep', ['1']);
  child.on('exit', function(code, signal) {
    if (code === 0) {
      const callback = (error, info) => {
        if (error) {
          console.log(error);
          res.send('error');
        } else {
          res.send('success');
        }
      }
      sendMail(req.body.email, distilled_file_name, callback);
    } else {
      res.send('fail');
    }
  });
});

app.listen(port, () => console.log(`Listening on port ${port}`));
