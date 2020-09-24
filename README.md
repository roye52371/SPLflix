"# SPLflix" 
 program that simulates a new streaming service -
SPLFLIX. SPLFLIX offers users two types of streaming content – movies and tv-episodes,
which belong to a given tv-series. Like most
streaming services we allow a creation of
multiple users, while each user receives userspecific recommendations, according to his
watch history.
However, on SPLFLIX, unlike current
streaming services, we give our users the
ability to choose which recommendation
algorithm to use, out of a number of algorithms
we offer. Each user stores its own watch history, and can use a different recommendation
algorithm. After the user chooses to watch a content, a recommendation can be given to the
user for the next content.
At each point during the session, there is only one active user (while other users are
considered as non-active), which means- only one user can watch SPLFLIX at any given point.
The program  receive a config file (json) as an input, which includes all the information
about the available streaming content .
