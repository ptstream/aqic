Name:           aqic
Version:        0.0.0  
Release:        1%{?dist}
Summary:        Air quality index browser
License:        Freeware  
AutoReqProv:    no
Requires:       /bin/sh
icon:           aqic64.xpm
group:          Utility

%description       
This application allows to give an elegant representation of the air quality index.

%post
#!/bin/sh
sudo chmod -R +rwxrxrx /opt/aqic
if [ -f /opt/aqic/aqic.desktop ]
then
echo "--------------> Creating application entry (aqic.desktop)"
cp /opt/aqic/aqic.desktop /usr/share/applications;
fi
exit 0

%preun
#!/bin/sh
if [ -f /opt/aqic/aqic.run ]
then
echo "--------------> Remove local data"
rm -R -d /home/*/.local/share/aqic
echo "--------------> Remove data cache"
rm -R -d /home/*/.cache/aqic /home/*/.cache/QtLocation
fi
exit 0

%postun
#!/bin/sh
if [ -f /usr/share/applications/aqic.desktop ]
then
echo "--------------> Remove application entry (aqic.desktop)."
rm /usr/share/applications/aqic.desktop;
fi
exit 0

%changelog
* Fri Nov  6 13:10:51 CET 2020 hillou
- 

%files
%defattr(755, root, root, -)
%license
%doc
