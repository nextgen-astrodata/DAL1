import dal

ds = dal.dalDataset("foo.h5")

g1 = ds.createGroup("GroupA")
g2 = ds.createGroup("GroupB")

t1 = ds.createTable("table1","GroupA")
t2 = ds.createTable("table2","GroupB")

t1.addColumn("col1", dal.dal_INT )
t1.addColumn("col2", dal.dal_FLOAT )
t1.addColumn("col3", dal.dal_FLOAT )
#t1.listColumns()

t2.addColumn("data", dal.dal_INT )
t2.listColumns()

pdata = dal.intp() # pointer to data

for x in range(100):
	pdata.assign(x) # assign poniter a value
	t2.appendRow(pdata) # write to table

outdata = dal.intp() # pointer to data

# read some values
for x in range(10,30,2): # 10 to 30, step 2
	t2.readRows(outdata,x,1)
	print outdata.value()

g1.setAttribute_int("intAttr",5)
g1.setAttribute_string("strAttr","teststring")
g1.setAttribute_uint("uintAttr",6)
g1.setAttribute_double("doubleAttr",4.321)

ds.close()
