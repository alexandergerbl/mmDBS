#
#   This file contains howto compile and run this repository correctly!
#


1. git clone https://github.com/alexandergerbl/mmDBS.git
2. run "make" in the directory of the Makefile
3. download the task1.gz file from the pracital course webpage (https://db.in.tum.de/teaching/ws1617/imlab/?lang=en) 
4. unpack task1.gz to task1 und put this directory in the same directory as the Makefile
5. After that your directory should look like example 1:

6. run the program with ./mmDBS

EXAMPLE 1:


.
├── ColumnStore.hpp
├── CustomerColumn.cpp
├── CustomerColumn.hpp
├── Customer.cpp
├── Customer.hpp
├── DatabaseColumn.cpp
├── DatabaseColumn.hpp
├── Database.cpp
├── Database.hpp
├── DistrictColumn.cpp
├── DistrictColumn.hpp
├── District.cpp
├── District.hpp
├── HistoryColumn.cpp
├── HistoryColumn.hpp
├── History.cpp
├── History.hpp
├── ItemColumn.cpp
├── ItemColumn.hpp
├── Item.cpp
├── Item.hpp
├── main.cpp
├── Makefile
├── mmDBS
├── mmDBS.kdev4
├── My_Hashes.hpp
├── NewOrderColumn.cpp
├── NewOrderColumn.hpp
├── NewOrder.cpp
├── NewOrder.hpp
├── neworderrandom.cpp
├── OrderColumn.cpp
├── OrderColumn.hpp
├── Order.cpp
├── Order.hpp
├── OrderLineColumn.cpp
├── OrderLineColumn.hpp
├── OrderLine.cpp
├── OrderLine.hpp
├── perf.data
├── perf.data.old
├── README.txt
├── Row.hpp
├── StockColumn.cpp
├── StockColumn.hpp
├── Stock.cpp
├── Stock.hpp
├── task1
│   ├── neworderrandom.cpp
│   ├── neworder.script
│   ├── oltp.script
│   ├── schema.sql
│   ├── tpcc_customer.tbl
│   ├── tpcc_district.tbl
│   ├── tpcc_history.tbl
│   ├── tpcc_item.tbl
│   ├── tpcc_neworder.tbl
│   ├── tpcc_orderline.tbl
│   ├── tpcc_order.tbl
│   ├── tpcc_stock.tbl
│   ├── tpcc_warehouse.tbl
│   ├── Types.cpp
│   └── Types.hpp
├── tests
│   ├── include
│   │   └── gtest
│   │       ├── gtest-death-test.h
│   │       ├── gtest.h
│   │       ├── gtest-message.h
│   │       ├── gtest-param-test.h
│   │       ├── gtest-param-test.h.pump
│   │       ├── gtest_pred_impl.h
│   │       ├── gtest-printers.h
│   │       ├── gtest_prod.h
│   │       ├── gtest-spi.h
│   │       ├── gtest-test-part.h
│   │       ├── gtest-typed-test.h
│   │       └── internal
│   │           ├── custom
│   │           │   ├── gtest.h
│   │           │   ├── gtest-port.h
│   │           │   └── gtest-printers.h
│   │           ├── gtest-death-test-internal.h
│   │           ├── gtest-filepath.h
│   │           ├── gtest-internal.h
│   │           ├── gtest-linked_ptr.h
│   │           ├── gtest-param-util-generated.h
│   │           ├── gtest-param-util-generated.h.pump
│   │           ├── gtest-param-util.h
│   │           ├── gtest-port-arch.h
│   │           ├── gtest-port.h
│   │           ├── gtest-string.h
│   │           ├── gtest-tuple.h
│   │           ├── gtest-tuple.h.pump
│   │           ├── gtest-type-util.h
│   │           └── gtest-type-util.h.pump
│   ├── make
│   │   ├── a.out
│   │   ├── gtest-all.o
│   │   ├── gtest_main.a
│   │   ├── gtest_main.o
│   │   ├── MakefileTests
│   │   ├── sample1.o
│   │   └── sample1_unittest.o
│   ├── samples
│   │   └── sample1_unittest.cpp
│   └── src
│       ├── gtest-all.cpp
│       ├── gtest.cpp
│       ├── gtest-death-test.cpp
│       ├── gtest-filepath.cpp
│       ├── gtest-internal-inl.h
│       ├── gtest_main.cpp
│       ├── gtest-port.cpp
│       ├── gtest-printers.cpp
│       ├── gtest-test-part.cpp
│       └── gtest-typed-test.cpp
├── Types.cpp
├── Types.hpp
├── WarehouseColumn.cpp
├── WarehouseColumn.hpp
├── Warehouse.cpp
└── Warehouse.hpp

9 directories, 114 files


