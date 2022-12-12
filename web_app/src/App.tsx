import React from 'react';
import './App.css';
import { PageLayout } from './components/PageLayout';
import { PageHeader } from './components/PageHeader';


function App() {

    const [page, setPage] = React.useState<number>(0);

    const changePage = (newPage: number) => {
        setPage(newPage);
    }
    //  TODO: remove spaces from email
    return (
        <div className="root">
            <PageHeader page={page} />
            <div className="upper-page">
                <PageLayout page={page} changePage={changePage} />
                <div className="wave">
                    <svg width="100%" height="200px" fill="none" version="1.1"
                        xmlns="http://www.w3.org/2000/svg">
                        <path
                            fill="white"
                            d="
                            M0 67
                            C 273,183
                            822,-41
                            1920.01,106 
                            V 359 
                            H 0 
                            V 67
                            Z">
                        </path>
                    </svg>
                </div>
            </div>
        </div>
    );
}

export default App;
