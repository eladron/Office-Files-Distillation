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
        data.append('id', content);
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
                <h2 className="form-title">ID:</h2>
                <label>
                    <input className="email-input" pattern="[0-9]{9}" maxLength={9} onChange={handleContentChange} required/>
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