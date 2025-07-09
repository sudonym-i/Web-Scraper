
WEBSCRAPER

-----------------------------------

Dependencies:
  -libCurl

-----------------------------------

The 'chain.txt' is the file you use to tell the program what to do. 
Specify the url(s), start tag(s), and end tag(s).
The program will go through the website and collect all of the content that lies
between these two tags. With javascript rendered sites it might take a bit more effort to 
find a start-tag that works.

The featureset is currenrly rather bare, however it's still an effective tool.

Currently, the data scraped from the specified websites will be saved to a file in the /build directory. The name can be changed in the "crawlchain.txt" file

----------------------------------
