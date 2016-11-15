To deploy the sbacs-api on AWS:

- First, create the source package. Go into the sbacs-api folder, then select all, then create a zip
	Name the zip appropriately, and move it somewhere so you don't accidentally commit it.
- Go to AWS Management Console and log in
- Go to Elastic Beanstalk
- There is a menu bar on the top, with SBACS as an option, select that
- You will see a drop down menu, select 'Application Versions'
- Select 'Upload', give a meaningful name (with a version number that is appropriate),
	browse for the zip you created earlier, and upload that
- The newly uploaded code will appear on the list with the name you gave it
- Select the new version, then select 'Deploy'

If you encounter issues with this new version, please go back to the 'Application Versions'
	page and deploy the previously working version, so we have a working version at all times.
