const express = require('express');
const multer = require('multer');
const fs = require('fs');
const path = require('path');
const {spawn, execFile} = require('child_process');
const app = express();
const port = process.env.PORT || 5000;

const upload = multer({dest: 'uploads/'});
let i = 0;

app.post('/api/upload', upload.single('file'), (req, res) => {
  const new_file_name = `uploads/document${i}.docx`;
  const distilled_file_name = `distilled/document${i}.docx`;
  i++;
  fs.rename(req.file.path, new_file_name, function(err) {
    if (err) {
      console.log(err);
    }
  });
  const distill_file_path = path.resolve(__dirname, '../Distill\ Module/distill')
  const args = [`${new_file_name}`, `${distilled_file_name}`]
  execFile(distill_file_path, args, (error, stdout, stderr) => {
    if (error === null) {

      const mail_callback = (error, stdout, stderr) => {
        if (error.code !== 0) {
          console.log(error);
          res.send('failed to send email');
        } else {
          res.send('email sent');
        }
      }
      spawn("python3", ["send_email.py", req.body.email, distilled_file_name], {shell: true}, mail_callback);
    } else {
      res.send('sanitization failed');
    }
  });

});

app.listen(port, () => console.log(`Listening on port ${port}`));
