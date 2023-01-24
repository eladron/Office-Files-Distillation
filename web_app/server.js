const express = require('express');
const multer = require('multer');
const fs = require('fs');
const path = require('path');
const {spawn, execFile} = require('child_process');
const app = express();
const port = process.env.PORT || 5000;

// create directory for uploads
if (!fs.existsSync('./uploads')) {
  fs.mkdirSync('./uploads');
}
// create directory for distilled files
if (!fs.existsSync('./distilled')) {
  fs.mkdirSync('./distilled');
}


const upload = multer({dest: __dirname + 'uploads/'});
let i = 0;

app.post('/api/upload', upload.single('file'), (req, res) => {
  const new_file_name = __dirname + `/uploads/document${i}.docx`;
  const distilled_file_name = __dirname + `/distilled/document${i}.docx`;
  i++;
  fs.rename(req.file.path, new_file_name, function(err) {
    if (err) {
      console.log(err);
    }
  });
  const distill_file_path =
      path.resolve(__dirname, '../distillation_module/distill')
  const args = [`${new_file_name}`, `${distilled_file_name}`]
  console.log('distilling file...')
  console.log(distill_file_path)
  for (let i = 0; i < args.length; i++) {
    console.log(args[i])
  }
  execFile(distill_file_path, args, (error, stdout, stderr) => {
    if (error === null) {
      let mailer = spawn(
          'python3',
          [
            `${__dirname}/email_handler.py`, req.body.email,
            distilled_file_name, req.body.id
          ],
          {shell: true},
      );
      mailer.stdout.on('data', function(data) {
        console.log(data.toString());
      });
      mailer.stderr.on('data', function(data) {
        console.log(data.toString());
      });
      mailer.on('exit', function(code, signal) {
        if (code !== 0) {
          console.log('email could not be sent')
          res.send('failed to send email');
        } else {
          console.log('email sent!')
          res.send('email sent');
        }
      })
    } else {
      console.log(error);
      console.log('sanitization failed')
      res.send('sanitization failed');
    }
  });
});

app.listen(port, () => console.log(`Listening on port ${port}`));