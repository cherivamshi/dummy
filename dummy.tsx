import React, { useState } from 'react';
import styles from '../../../../../src/questionnarie/QuestionnarieHome/questionnarie.module.css';
import {
  WppIconDownload,
  WppActionButton,
} from '@eplatform-ui-kit/components-library-react';
import { useQuery } from '@tanstack/react-query';
import {
  AudienceOriginRequest,
  EndPoints,
} from '../../api/audience-origin';

// FetchDownloadData function
const FetchDownloadData = async ({ surveyName, fileName }) => {
  const { data } = await AudienceOriginRequest.post(
    `${EndPoints.DownloadFiles}?fileName=${fileName}&survey=${surveyName}`
  );
  return data;
};

// DownloadButtonComponent component
const DownloadButtonComponent = ({ event }) => {
  const [downloadInfo, setDownloadInfo] = useState(null);

  const onDownloadClick = async (event) => {
    const surveyName = event.target.closest('.ag-row').childNodes[0].textContent;
    const fileName = event.target.closest('.ag-row').childNodes[1].textContent;

    setDownloadInfo({ surveyName, fileName });

    try {
      // Fetch the data
      const data = await FetchDownloadData({ surveyName, fileName });

      // Check if data is fetched successfully
      if (!data) {
        console.error('Download failed: No data received');
        // Optionally, display an error message to the user
        return;
      }

      // Convert data to CSV format
      const csvData = convertToCSV(data);

      // Create a downloadable link
      const blob = new Blob([csvData], { type: 'text/csv;charset=utf-8;' });
      const link = document.createElement('a');
      const url = URL.createObjectURL(blob);
      link.setAttribute('href', url);
      link.setAttribute('download', `${fileName}.csv`);
      link.style.visibility = 'hidden';
      document.body.appendChild(link);
      link.click();
      document.body.removeChild(link);
    } catch (error) {
      console.error('Download error:', error);
      // Display an error message to the user (optional)
    }
  };

  const convertToCSV = (data) => {
    // Modify this function if the fetched data structure is different
    const array = [Object.keys(data[0])].concat(data);
    return array.map(row => Object.values(row).join(',')).join('\n');
  };

  return (
    <div slot="actions" className={styles['card-download']}>
      <WppActionButton onClick={onDownloadClick} variant="secondary">
        <WppIconDownload slot="icon-start" />
      </WppActionButton>
    </div>
  );
};

export default DownloadButtonComponent;
