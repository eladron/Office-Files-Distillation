import React from 'react';
import '../App.css';


export interface pageContentProps {
    changePage: (newPage: number) => void;
}


export const PageContent: React.FC<pageContentProps> = ({
    changePage
}) => {
    const [uploadedFile, setUploadedFile] = React.useState(null);
    const [email, setEmail] = React.useState<string>("");
    const [content, setContent] = React.useState<string>("");


    const handleEmailChange = (event: any) => {
        setEmail(event.target.value);
    }

    const handleContentChange = (event: any) => {
        setContent(event.target.value);
    }

    const handleFileChange = (event: any) => {
        console.log(event.target.files[0]);
        setUploadedFile(event.target.files[0]);
    }

    const handleSubmit = async (event: any) => {
        event.preventDefault();
        const data = new FormData();
        data.append('file', uploadedFile ? uploadedFile : "");
        data.append('email', email);
        const response = await fetch('/api/upload', {
            method: 'POST',
            body: data
        });
        const body = await response.text();
        console.log(body);
        changePage(1);
    }

    return (
        <div className="form-container">
            <form onSubmit={handleSubmit} className="form">
                <h2 className="form-title">Email:</h2>
                <label>
                    <input className="email-input" type="email" onChange={handleEmailChange}
                        pattern="[a-z0-9._%+-]+@[a-z0-9.-]+\.[a-z]{2,4}$" />
                </label>
                <h2 className="form-title">Type:</h2>
                <div className="custom-select" style={{ width: "200px" }}>
                    <select>
                        <option value="0">Request</option>
                        <option value="1">Medical Report</option>
                        <option value="2">Certificate</option>
                        <option value="3">Other</option>
                    </select>
                </div>
                <h2 className="form-title">Content:</h2>
                <label>
                    <input className="email-input" type="email" onChange={handleContentChange} />
                </label>
                <h2 className="form-title">File:</h2>
                <label>
                    <input className="file-input" type="file" accept=".docx" onChange={handleFileChange} required />
                </label>
                <button className="submit-button" type="submit" >Submit</button>
            </form >
            <div id="text-container" className="text-container">
                <h1 style={{ fontSize: "40px" }}>So how does it work?</h1>
                <h2>To send a file to the system, all you need to do is: </h2>
                <h2>Supply an email address at the Email specified textbox. </h2>
                <h2>Upload a file, and hit the 'Submit' button. </h2>
            </div>
        </div >

    )
}