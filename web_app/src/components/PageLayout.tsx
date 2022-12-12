import React from 'react';
import '../App.css';
import { PageContent } from './pageContent';
import { ResponsePage } from './ResponsePage';

export interface PageLayoutProps {
    page: number;
    changePage: (newPage: number) => void;
}
export const PageLayout: React.FC<PageLayoutProps> = ({
    page, changePage
}) => {

    switch (page) {
        case 0:
            return (<div>
                <h1 className="title">File Sanitizing</h1>
                <PageContent changePage={changePage} />
            </div>);
        case 1:
            return (<ResponsePage changePage={changePage} />);
        default:
            return null;

    }
}
