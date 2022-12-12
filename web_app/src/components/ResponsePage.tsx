import React from 'react';
import '../App.css';

export interface ResponsePageProps {
    changePage: (newPage: number) => void;
}
export const ResponsePage: React.FC<ResponsePageProps> = ({
    changePage
}) => {
    return (
        <div className="final-message">
            <h1 className="title">Thank You!</h1>
            <h2 className="title" style={{ fontSize: '40px' }}>Your file has been uploaded successfully.</h2>
            <button className="return-button" onClick={() => changePage(0)}>Return to send another file</button>
        </div>
    )
}
