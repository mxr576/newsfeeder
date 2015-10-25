/**
 * @file
 * Heatmap.js integration helper for Drupal.
 */
(function ($) {
  Drupal.behaviors.leaflet_heatmap = {
    attach: function (context, settings) {
      var heatmapContainer = $('#leaflet-heatmap', context);
      // Check, if the container exists.
      if (heatmapContainer.length > 0) {
        // Settings from the original example.
        // @see http://www.patrick-wied.at/static/heatmapjs/example-heatmap-leaflet.html
        var baseLayer = L.tileLayer('http://{s}.basemaps.cartocdn.com/light_nolabels/{z}/{x}/{y}.png', {
          attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors, &copy; <a href="http://cartodb.com/attributions">CartoDB</a>',
          maxZoom: 18
        });
        var cfg = {
          // radius should be small ONLY if scaleRadius is true (or small radius is intended)
          // if scaleRadius is false it will be the constant radius used in pixels
          "radius": 3,
          "maxOpacity": 0.8,
          // scales the radius based on map zoom
          "scaleRadius": true,
          // if set to false the heatmap uses the global maximum for colorization
          // if activated: uses the data maximum within the current map boundaries
          //   (there will always be a red spot with useLocalExtremas true)
          "useLocalExtrema": true,
          // which field name in your data represents the latitude - default "lat"
          latField: 'lat',
          // which field name in your data represents the longitude - default "lng"
          lngField: 'lng',
          // which field name in your data represents the data value - default "value"
          valueField: 'count'
        };

        var heatmapLayer = new HeatmapOverlay(cfg);

        var map = L.map('leaflet-heatmap', {
          scrollWheelZoom: false,
          layers: [baseLayer, heatmapLayer]
        }).setView([25, 0], 2);

        heatmapLayer.setData({data: settings.leaflet_heatmap[0].features});
      }
    }
  };
})(jQuery);
