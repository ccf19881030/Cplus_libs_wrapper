mLualib = require "mLualib"
local avg, sum = mLualib.average(1,2,3,4,5,6,7,8,9,10)	-- 参数对应堆栈中的数据
print(avg, sum)	-- 5.5 55.0
mLualib.sayHello() -- hello World!
