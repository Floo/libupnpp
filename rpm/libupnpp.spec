Summary:        C++ wrapper for libupnp
Name:           libupnpp
Version:        0.12.1
Release:        1%{?dist}
Group:          Applications/Multimedia
License:        GPLv2+
URL:            http://www.lesbonscomptes.com/updmpdcli
Source0:        http://www.lesbonscomptes.com/upmpdcli/downloads/libupnpp-%{version}.tar.gz
Requires(pre):  shadow-utils
Requires(post): systemd
Requires(preun): systemd
Requires(postun): systemd
BuildRequires:  libupnp-devel
BuildRequires:  libmpdclient-devel
BuildRequires:  expat-devel
BuildRequires:  libcurl-devel
BuildRequires:  systemd-units
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
libupnpp is a C++ wrapper over libupnp. It exists mostly for supporting
upmpdcli and upplay

%prep
%setup -q

%build
%configure
%{__make} %{?_smp_mflags}

%install
%{__rm} -rf %{buildroot}
%{__make} install DESTDIR=%{buildroot} STRIP=/bin/true INSTALL='install -p'
%{__rm} -f %{buildroot}%{_libdir}/libupnpp.a
%{__rm} -f %{buildroot}%{_libdir}/libupnpp.la

%clean
%{__rm} -rf %{buildroot}

%files
%defattr(-, root, root, -)
%{_includedir}/libupnpp
%{_libdir}/libupnpp.so*

%changelog
* Mon Aug 17 2015 J.F. Dockes <jf@dockes.org> - 0.12.1
- Make it easier to create pure openhome devices
* Tue May 05 2015 J.F. Dockes <jf@dockes.org> - 0.11.0
- Control side interface to the OpenHome receiver service
- API cleanups for a more stable ABI
- More complete implementation of OpenHome services to support pure
  OpenHome devices
* Sun Oct 12 2014 J.F. Dockes <jf@dockes.org> - 0.8.4
- Separation from upmpdcli
