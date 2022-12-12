import React from 'react';
import '../App.css';

export interface PageHeaderProps {
    page: number;
}
export const PageHeader: React.FC<PageHeaderProps> = ({
    page
}) => {
    if (page === 0) {
        return (<div>
            <a className="header-button" href="#text-container">?</a>
        </div>);
    } else {
        return null;
    }
}

