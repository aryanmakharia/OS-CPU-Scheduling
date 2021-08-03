# Operating-Systems-CPU-Scheduling

Designed and implemented a novel CPU scheduling algorithm in C++ language.


Motivation:
- As Shortest Job First (SJF) helps to reduce the Average Waiting Time (AWT) and Average Turnaround Time (ATT), we have incorporated it in our algorithm. But it induces a problem of starvation.
- To avoid this problem, we added the Round Robin (RR) algorithm which will help us to control the problem of starvation to some extent.
- Also, to further improve the starvation problem, we have incorporated an additional priority for longer processes. This priority only activates if the user specifically mentions that the starvation has to be reduced by a significant amount.
Note: Giving a priority to longer processes will help reduce the starvation problem but it will increase the AWT and ATT by some extent.



Algorithm:
- We are using the Round Robin (RR) algorithm along with the Shortest Job First (SJF) algorithm.
- The quanta for the RR algorithm is dynamically chosen after every round. It is calculated by taking the mean of all the processes available in the ready queue.
- The quanta changes only after a cycle is ended, that is, until all the processes in the ready queue have finished one round along with the new processes that arrived during the execution of the processes in the ready queue.
- The processes in the ready queue are always sorted before execution so that the shortest process in the ready queue is executed first.
- Our Algorithm has a special provision of taking into account if we need large processes to be executed relatively earlier. This does not mean that the algorithm will execute the whole process at once, rather it will execute the process for an extra quanta. Basically, if any process in ready queue has its remaining burst time greater than 3*mean (mean of all the processes available in the ready queue) we will execute it twice in one single round. It will make sure even the extreme outliers aren’t the ones executed at last (if we want so).
Example: If the mean of all the available processes in the queue is 3 and one of the processes has a remaining burst time of 15, this particular process will be executed twice, resulting in its new value to be 15 - 2*3 = 9.


Advantages:
- Reduces Avg. Waiting Time and Avg. Turnaround Time compared to Round Robin in general. For some cases better AWT and ATT compared to SJF too.
- Longer processes aren't necessarily the ones executed last, which is the case for both Round Robin and Shortest Job First.
- Incorporation of the RR algorithm helps to control the problem of starvation.
- Dynamic Quanta allocation helps the algorithm to work with processes having
large burst time as well as short burst time. With this, quanta will automatically be calculated depending on the actual burst time. This will prevent the errors due to choosing of quanta size before actually knowing about the incoming processes. Example: If we fix the quanta in the RR algorithm to 10, then it would give us a bad result if the burst time of processes are in the range 2 to 8. Similarly, if we choose a small quanta size, it will result in unnecessary context switching.
- Calculating mean instead of min/max helps in proper quanta. As we won't just be ending a single shortest process/converting our algo to execute all in ready queue but weighting them all for have sufficient quanta for larger processes and not extremely high either as that would increase waiting time for remaining short processes
- Choosing the smallest available process by sorting after every cycle helps in reduction of Avg. Waiting Time and Avg. Turnaround Time.
