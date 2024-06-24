import { AgGridReact } from "ag-grid-react";
import { MetadataDiffSheetItemBase } from "../../../types";
import React, { useRef, useState } from "react";
import { WppPagination, WppSpinner, PaginationChangeEventDetail } from "@platform-ui-kit/components-library-react";

export const MetadataDiffGrid = (props: { data: MetadataDiffSheetItemBase[]; columns: any[] }) => {
  const gridRef = useRef<AgGridReact>(null);
  const [paginationPageSize, setPaginationPageSize] = useState<number>(5);
  const [currPage, setPage] = useState<number>(1);

  const handlePaginationChange = (event: CustomEvent<PaginationChangeEventDetail>) => {
    const { itemsPerPage, page } = event.detail;

    gridRef.current!.api.showLoadingOverlay();
    if (paginationPageSize !== itemsPerPage) {
      setPaginationPageSize(itemsPerPage);
      gridRef.current!.api.paginationSetPageSize(itemsPerPage);
    }
    if (currPage !== page) {
      gridRef.current!.api.paginationGoToPage(page - 1);
      setPage(page);
    }
    gridRef.current!.api.hideOverlay();
  };

  function onGridReady(params: any) {
    this.gridApi = params.api;
    this.gridColumnApi = params.columnApi;
    this.gridApi.sizeColumnsToFit();
    // Auto size columns to fit their content
    const allColumnIds = [];
    this.gridColumnApi.getAllColumns().forEach((column: any) => {
      allColumnIds.push(column.colId);
    });
    this.gridColumnApi.autoSizeColumns(allColumnIds);
  }

  return (
    <div style={{ width: "auto", marginBottom: 20 }} className="ag-theme-wpp">
      <AgGridReact
        ref={gridRef}
        rowData={props.data}
        columnDefs={props.columns}
        headerHeight={40}
        rowHeight={40}
        pagination={true}
        paginationPageSize={paginationPageSize}
        suppressPaginationPanel={true}
        loadingOverlayComponent={() => <WppSpinner size="m" />}
        domLayout={"autoHeight"}
        onGridReady={onGridReady}
        resizable={true} // Enable column resizing
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
