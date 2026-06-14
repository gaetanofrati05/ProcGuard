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
 <h2>Here is the division of the project</h2>
 <table>
    <thead>
      <tr>
        <th>Data Structer/Gathering of anomaly</th>
        <th></th>
      </tr>
      
   
 </table>
