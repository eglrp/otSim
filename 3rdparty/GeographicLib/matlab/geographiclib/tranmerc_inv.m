function [lat, lon, gam, k] = tranmerc_inv(lat0, lon0, x, y, ellipsoid)
%TRANMERC_INV  Inverse transverse Mercator projection
%
%   [lat, lon] = TRANMERC_INV(lat0, lon0, x, y)
%   [lat, lon, gam, k] = TRANMERC_INV(lat0, lon0, x, y, ellipsoid)
%
%   performs the inverse transverse Mercator projection of points (x,y) to
%   (lat,lon) using (lat0,lon0) as the center of projection.  These input
%   arguments can be scalars or arrays of equal size.  The ellipsoid vector
%   is of the form [a, e], where a is the equatorial radius in meters, e is
%   the eccentricity.  If ellipsoid is omitted, the WGS84 ellipsoid (more
%   precisely, the value returned by defaultellipsoid) is used.  The common
%   case of lat0 = 0 is treated efficiently provided that lat0 is specified
%   as a scalar.  projdoc defines the projection and gives the restrictions
%   on the allowed ranges of the arguments.  The forward projection is
%   given by tranmerc_fwd.  The scale on the central meridian is 1.
%
%   gam and K give metric properties of the projection at (lat,lon); gam is
%   the meridian convergence at the point and k is the scale.
%
%   lat0, lon0, lat, lon, gam are in degrees.  The projected coordinates x,
%   y are in meters (more precisely the units used for the equatorial
%   radius).  k is dimensionless.
%
%   This implementation of the projection is based on the series method
%   described in
%
%     C. F. F. Karney, Transverse Mercator with an accuracy of a few
%     nanometers, J. Geodesy 85(8), 475-485 (Aug. 2011);
%     Addenda: https://geographiclib.sourceforge.io/tm-addenda.html
%
%   This extends the series given by Krueger (1912) to sixth order in the
%   flattening.  This is a substantially better series than that used by
%   the MATLAB mapping toolbox.  In particular the errors in the projection
%   are less than 5 nanometers withing 3900 km of the central meridian (and
%   less than 1 mm within 7600 km of the central meridian).  The mapping
%   can be continued accurately over the poles to the opposite meridian.
%
%   See also PROJDOC, TRANMERC_FWD, UTMUPS_FWD, UTMUPS_INV,
%     DEFAULTELLIPSOID.

% Copyright (c) Charles Karney (2012-2015) <charles@karney.com>.

  narginchk(4, 5)
  if nargin < 5, ellipsoid = defaultellipsoid; end
  try
    S = size(lat0 + lon0 + x + y);
  catch
    error('lat0, lon0, x, y have incompatible sizes')
  end
  if length(ellipsoid(:)) ~= 2
    error('ellipsoid must be a vector of size 2')
  end

  Z = zeros(prod(S),1);
  maxpow = 6;

  a = ellipsoid(1);
  e2 = real(ellipsoid(2)^2);
  f = e2 / (1 + sqrt(1 - e2));
  e2m = 1 - e2;
  cc = sqrt(e2m) * exp(eatanhe(1, e2));
  n = f / (2 -f);
  bet = betf(n);
  b1 = (1 - f) * (A1m1f(n) + 1);
  a1 = b1 * a;

  if isscalar(lat0) && lat0 == 0
    y0 = 0;
  else
    [sbet0, cbet0] = sincosdx(LatFix(lat0(:)));
    [sbet0, cbet0] = norm2((1-f) * sbet0, cbet0);
    y0 = a1 * (atan2(sbet0, cbet0) + ...
               SinCosSeries(true, sbet0, cbet0, C1f(n)));
  end
  y = y(:) + y0 + Z;

  xi = y / a1;
  eta = x(:) / a1 + Z;
  xisign = 1 - 2 * (xi < 0 );
  etasign = 1 - 2 * (eta < 0 );
  xi = xi .* xisign;
  eta = eta .* etasign;
  backside = xi > pi/2;
  xi(backside) = pi - xi(backside);

  c0 = cos(2 * xi); ch0 = cosh(2 * eta);
  s0 = sin(2 * xi); sh0 = sinh(2 * eta);
  ar = 2 * c0 .* ch0; ai = -2 * s0 .* sh0;
  j = maxpow;
  xip0 = Z; yr0 = Z;
  if mod(j, 2)
    xip0 = xip0 + bet(j);
    yr0 = yr0 - 2 * maxpow * bet(j);
    j = j - 1;
  end
  xip1 = Z; etap0 = Z; etap1 = Z;
  yi0 = Z; yr1 = Z; yi1 = Z;
  for j = j : -2 : 1
    xip1  = ar .* xip0 - ai .* etap0 - xip1 - bet(j);
    etap1 = ai .* xip0 + ar .* etap0 - etap1;
    yr1 = ar .* yr0 - ai .* yi0 - yr1 - 2 * j * bet(j);
    yi1 = ai .* yr0 + ar .* yi0 - yi1;
    xip0  = ar .* xip1 - ai .* etap1 - xip0 - bet(j-1);
    etap0 = ai .* xip1 + ar .* etap1 - etap0;
    yr0 = ar .* yr1 - ai .* yi1 - yr0 - 2 * (j-1) * bet(j-1);
    yi0 = ai .* yr1 + ar .* yi1 - yi0;
  end
  ar = ar/2; ai = ai/2;
  yr1 = 1 - yr1 + ar .* yr0 - ai .* yi0;
  yi1 =   - yi1 + ai .* yr0 + ar .* yi0;
  ar = s0 .* ch0; ai = c0 .* sh0;
  xip  = xi  + ar .* xip0 - ai .* etap0;
  etap = eta + ai .* xip0 + ar .* etap0;
  gam = atan2dx(yi1, yr1);
  k = b1 ./ hypot(yr1, yi1);
  s = sinh(etap);
  c = max(0, cos(xip));
  r = hypot(s, c);
  lon = atan2dx(s, c);
  sxip = sin(xip);
  tau = tauf(sxip./r, e2);
  lat = atan2dx(tau, 1 + Z);
  gam = gam + atan2dx(sxip .* tanh(etap), c);
  c = r ~= 0;
  k(c) = k(c) .* sqrt(e2m + e2 ./ (1 + tau.^2)) .* ...
         hypot(1, tau(c)) .* r(c);
  c = ~c;
  if any(c)
    lat(c) = 90;
    lon(c) = 0;
    k(c) = k(c) * cc;
  end
  lat = lat .* xisign;
  lon(backside) = 180 - lon(backside);
  lon = lon .* etasign;
  lon = AngNormalize(lon + AngNormalize(lon0(:)));
  gam(backside) = 180 - gam(backside);
  gam = AngNormalize(gam .* xisign .* etasign);

  lat = reshape(lat, S); lon = reshape(lon, S);
  gam = reshape(gam, S); k = reshape(k, S);
end

function bet = betf(n)
  persistent betcoeff
  if isempty(betcoeff)
    betcoeff = [
        384796, -382725, -6720, 932400, -1612800, 1209600, 2419200, ...
        -1118711, 1695744, -1174656, 258048, 80640, 3870720, ...
        22276, -16929, -15984, 12852, 362880, ...
        -830251, -158400, 197865, 7257600, ...
        -435388, 453717, 15966720, ...
        20648693, 638668800, ...
               ];
  end
  maxpow = 6;
  bet = zeros(1, maxpow);
  o = 1;
  d = n;
  for l = 1 : maxpow
    m = maxpow - l;
    bet(l) = d * polyval(betcoeff(o : o + m), n) / betcoeff(o + m + 1);
    o = o + m + 2;
    d = d * n;
  end
end
