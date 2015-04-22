#xml2header is a "standalone" Application, which transformes a xmlfile into a C++ usable headerfile

# Introduction #

In the first steps, xml2header is only usable manualy. There are some restrictions:
- For our usage we only need one xml File. So this xmlfile is hardcoded and named objects.xml and has to be in the folder res of the project

- In our next steps, xml2header will also be usable for more than one xmlfile. We will need that four our upcoming translation engine. There will be also the possibility to start xml2header with the make process.

To make sure xml2header works, please check if the file objects.xml is in the /res folder.