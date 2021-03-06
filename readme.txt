工程结构说明：

第一周	双任务切换操作系统的实现
C1.04	工程创建。标志变量反转的逻辑仪观测标志变量反转的逻辑仪观测

C1.07	内核编程实践。CPU寄存器数据的保护（压栈）与恢复（出栈），调试技术的应用。简单汇编代码的编写

C2.01	简单的任务定义实现。包括任务函数、任务结构体、任务栈的定义

C2.02	双任务切换的具体实现。应对任务切换过程中现场保护与现场恢复的实质内涵做深入思考；在各任务函数中调用任务调度函数来实现下一任务的选择及任务切换

C2.03	双任务时间片运行原理。通过在SysTick定时器（《Cortex-M3权威指南》第8章）的周期性定时溢出中断服务函数中调用任务调度函数实现

C2.04	双任务延时原理与空闲任务。通过基于SysTick定时器（时基单元提供者）的软定时器实现延时

第二周	核心数据结构及任务调度管理
C3.01	临界区保护。采用关闭中断的方式保护任务与任务之间、任务与中断之间的共享资源

C3.02	调度锁保护。设置一个任务调度锁计数器变量schedLockCount，若其值大于0，则任务调度函数是被禁止的

C3.03	位图数据结构。寻找位图中第一个置位的位置有两种方法：移位测试法（较慢）和分组查表法（较快）

C3.04	多优先级任务。每个任务都有一个优先级，将任务优先级、任务优先级列表（任务就绪表）中任务的位置、任务优先级位图中相应位的置位与否一一对应起来

C3.05	双向链表数据结构。学习了链表数据结构，应当理解前驱、后继、头节点等概念，后面还要学习队列数据结构

C3.06	任务延时队列。将延时未结束的任务插入任务延时队列，将SysTick定时中断服务函数中最初的扫描任务列表的方式改为扫描任务延时队列的方式，提升了程序执行效率

C3.07	同优先级时间片运行。加入了时间片轮转（round-robin）机制，每个任务都带有一个时间片计数器。但应注意的是，同优先级的多任务采用时间片运行时，各任务内部的延时
		函数只能采用软延时的方法，不能采用SysTick的定时延时
		
C4.01	任务的挂起与唤醒。挂起任务只是将任务从任务就绪表taskTable中的对应链表暂时移出的过程

C4.02	任务的删除。两种删除方式：强制删除（清理回调函数）和请求删除（判断是否被请求删除，是的话执行删除自身的操作）

C4.03	任务的状态查询。具体查询某一任务的状态时，只是将该任务结构体中的某些信息读取到任务信息查询结构体中

第三周	事件控制块、计数信号量、邮箱及存储块。
C5.01	事件控制块的原理与创建。事件控制块结构体里含有任务在等待的事件类型和任务等待列表，每一个任务结构体都含有一个事件控制块

C5.02	事件的等待与通知。将某事件控制块设为等待后，相关任务便会被插入事件控制块的等待列表中

C5.03	事件控制块的清空与状态查询。清空操作，即将事件控制块等待列表中的任务移出，并插入到任务就绪表；状态查询操作，返回事件控制块等待列表中的任务数量

C6.01	计数信号量的原理与创建。计数信号量由事件控制块和计数值（当前计数值与最大计数值）组成

C6.02	计数信号量的获取与释放。实现的三个函数接口：计数信号量等待（tSemWait）、计数信号量无等待获取（tSemNoWaitGet）、计数信号量通知（tSemNotify）

C6.03	计数信号量的删除与状态查询。删除操作，将计数信号量事件控制块任务等待列表中的任务全部删除，将计数值清零；状态查询操作，获取计数信号量的当前计数值、最大计
		数值、事件控制块任务等待列表中的任务数量等数据。

C7.01	邮箱的原理与创建。












