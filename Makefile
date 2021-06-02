DIST=norbert-robot-mapper
DIST_ROS_PACKAGE_PATH=/home/ubuntu/catkin_ws
LOCAL_ROS_PACKAGE_PATH=catkin_ws/src

DIST_MACHINE=ubuntu@192.168.1.50
DEB_PATH=/tmp

PACKAGE_NAME=norbert-robot-mapper

prepare_package:
	mkdir -p $(DIST)/$(DIST_ROS_PACKAGE_PATH)
	cp -r $(LOCAL_ROS_PACKAGE_PATH) $(DIST)/$(DIST_ROS_PACKAGE_PATH)
	dpkg-deb --build $(DIST) $(DIST)/$(PACKAGE_NAME).deb

deploy:
	scp $(DIST)/$(PACKAGE_NAME).deb $(DIST_MACHINE):$(DEB_PATH)
	ssh -t $(DIST_MACHINE) 'sudo dpkg -i $(DEB_PATH)/$(PACKAGE_NAME).deb'

clean:
	find ./$(DIST) -mindepth 1 ! -regex '^./$(DIST)/DEBIAN\(/.*\)?' -delete
