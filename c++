import { AgGridReact } from "ag-grid-react";
import { MetadataDiffSheetItemBase } from "../../../types";
import React, { useRef, useState, useCallback } from "react";
import { WppPagination, WppSpinner, PaginationChangeEventDetail } from "@platform-ui-kit/components-library-react";

export const MetadataDiffGrid = (props: { data: MetadataDiffSheetItemBase[]; columns: any[] }) => {
  const gridRef = useRef<AgGridReact>(null);
  const [paginationPageSize, setPaginationPageSize] = useState<number>(5);
  const [currPage, setPage] = useState<number>(1);

  const handlePaginationChange = (event: CustomEvent<PaginationChangeEventDetail>) => {
    const { itemsPerPage, page } = event.detail;

    if (gridRef.current) {
      gridRef.current.api.showLoadingOverlay();

      if (paginationPageSize !== itemsPerPage) {
        setPaginationPageSize(itemsPerPage);
        gridRef.current.api.paginationSetPageSize(itemsPerPage);
      }

      if (currPage !== page) {
        gridRef.current.api.paginationGoToPage(page - 1);
        setPage(page);
      }

      gridRef.current.api.hideOverlay();
    }
  };

  const onGridReady = useCallback((params: any) => {
    const gridApi = params.api;
    const gridColumnApi = params.columnApi;

    gridApi.sizeColumnsToFit();

    // Auto size columns to fit their content
    const allColumnIds = gridColumnApi.getAllColumns().map((column: any) => column.colId);
    gridColumnApi.autoSizeColumns(allColumnIds);
  }, []);

  // Add resizable property to each column
  const columnsWithResizing = props.columns.map(col => ({
    ...col,
    resizable: true
  }));

  return (
    <div style={{ width: "100%", marginBottom: 20 }} className="ag-theme-wpp">
      <AgGridReact
        ref={gridRef}
        rowData={props.data}
        columnDefs={columnsWithResizing}
        headerHeight={40}
        rowHeight={40}
        pagination={true}
        paginationPageSize={paginationPageSize}
        suppressPaginationPanel={true}
        overlayLoadingTemplate={'<span class="ag-overlay-loading-center">Please wait while your rows are loading</span>'}
        domLayout={"autoHeight"}
        onGridReady={onGridReady}
      />
      <WppPagination
        count={props.data.length}
        itemsPerPage={[5, 10, 15]}
        activePageNumber={currPage}
        pageSelectThreshold={9}
        data-testid="pagination-list"
        selectedItemPerPage={paginationPageSize}
        onWppChange={handlePaginationChange}
      />
    </div>
  );
};
