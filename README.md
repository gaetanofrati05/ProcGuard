<h1>Proc guard a new demon for your security</h1>

<p>
  This is a project created by me and consist to implement a demon for gathering of suspicious process. The project is completply writenn in C language 
  with management of safe memory, race condition, security options.

</p>

<h2>What it does?</h2>
 <ul>
   <li>It hooks to system with /proc without kernel mode is all userspace</li>
   <li>Monitor fork of process, opening of specific files, escaletion of privilegies </li>  
   <li>It observes anomaly for exmaple</li>
     <ol>
       <li>A process opens /etc/password</li>
       <li>Some who launches unfourseen shell</li>
       <li>PID injection attempts</li>
     </ol>
   <li>It writes a log file (hash SHA-256 of each entry)</li>
   <li>It sends an alert by a Unix socket when there is an anomaly</li>
 </ul>
 <p> The project follow a several division of the work I had made and there is a final part of massive testing of programm.
 The implementaion of hash SHA-256 is not made by me personally but I had taken by a video on youtube and this is the link 
   <a href="#">click here to see the video</a>
   
 </p>
 foreword: In this project an anomaly of a process is simply based on a change of status not based on what the process is doing or what process can access, the permission, change eccetera of course in a real project this can be widely complex
 The idea of this project was clear create a system where each states is marked and durable 
 <h2>Here is the division of the project</h2>
 <table>
    <thead>
      <tr>
        <th>Data Structure/Gathering of anomaly</th>
        <th>Management of log file</th>
        <th>Management of proc dir</th>
        <th>Alert system and refactory and clean</th>
      </tr>
      <tr>The data structure used is a simple HashTabel the gathering of the anomaly is supported by an attribute, the snap, associated each process </tr>
      <tr>The log file is managed with several functions thread-safe for writing in the log the status of process</tr>
      <tr>The proc dir is constaly read to screen the state of the process continuosly and if the process is suspicios it called send_alert function appends the log entry into the file and remove the pid of that specific process</tr>
      <tr>Alert system is supported by a socket which sends the alert on a message of the process that had observed the anomaly </tr>
 </table>
 
