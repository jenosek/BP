clear all 
close all
clc
%% Parameters
altitude = 5e5;
min_elevation = 5;

%% Calculated result
max_distance_calc = 2077;
disp("Maximum distance:  " + max_distance_calc + "km");

%% Reference
max_distance = slantRangeCircularOrbit(min_elevation, altitude, 0)/1e3;
disp("Maximum distance calculated by MATLAB:  " + max_distance + " km");

%% Calculation error
calculation_error = (max_distance_calc - max_distance) * 100 / max_distance; 
disp("Relative error of calculation = " + calculation_error + " %");