/**
 * @author
 * Árpád Csányi (arpad.csanyi@pronovix.com)
 */
(function ($) {
  d3.selection.prototype.moveToFront = function () {
    return this.each(function () {
      this.parentNode.appendChild(this);
    });
  };

  Drupal.d3.custom_frequency_graph = function (select, settings) {

    var labels = [],
      key = settings.legend,
      rows = settings.rows,
      p = [10, 50, 70, 50],
      w = 900 - p[1] - p[3],
      h = 400,
      chart = {w: w * .65, h: h - p[0] - p[2]},
      legend = {w: w * .25, h: h},
      x = d3.scale.linear().domain([0, rows.length - 1]).range([0, chart.w]),
      y = d3.scale.linear().domain([0, maxValue(rows)]).range([chart.h, 0]),
      z = d3.scale.category20();

    var svg = d3.select('#' + settings.id).append("svg")
      .attr("width", w)
      .attr("height", h)
      .attr('class', 'container')
      .append("g");

    var graph = svg.append("g")
      .attr("class", "chart")
      .attr('height', chart.h)
      .attr("transform", "translate(" + p[3] + "," + p[0] + ")");

    var periodLines = graph.append('g').attr('class', 'period-lines-wrapper');
    var numOfPeriods = 5;
    for (var i = 1; i < numOfPeriods; i++) {
      periodLines.append('line')
        .attr('class', 'period-line')
        .attr('stroke-dasharray', '5,5')
        .attr('stroke', 'gray')
        .attr('stroke-width', 1)
        .attr('x1', i * chart.w / (numOfPeriods))
        .attr('y1', 0)
        .attr('x2', i * chart.w / (numOfPeriods))
        .attr('y2', chart.h);
    }

    /* Y AXIS */
    var rule = graph.selectAll("g.rule")
      .data(y.ticks(8))
      .enter().append("g")
      .attr("class", "rule")
      .attr("transform", function (d) {
        return "translate(0," + y(d) + ")";
      });

    rule.append("line")
      .attr("x2", chart.w)
      .attr('visibility', function (d) {
        return d ? 'hidden' : 'visible';
      })
      .style("stroke", function (d) {
        return d ? "#eee" : "#ccc";
      })
      .style("stroke-opacity", function (d) {
        return d ? .7 : null;
      });

    rule.append("text")
      .attr('class', 'axis-label y-axis-label')
      .attr("x", -15)
      .attr("dy", ".35em")
      .attr("text-anchor", "end")
      .text(d3.format(",d"));

    rule.on('mouseover', function () {
      d3.event.stopPropagation();
    });

    // Add X axis labels
    var xAxisLabelFrom = graph.append('text')
      .attr('class', 'axis-label x-axis-label')
      .text(rows[0][0])
      .attr('x', 0)
      .attr('y', chart.h)
      .attr('dy', '15px')
      .attr('text-anchor', 'start');
    var xAxisLabelTo = graph.append('text')
      .attr('class', 'axis-label x-axis-label')
      .text(rows[rows.length - 1][0])
      .attr('x', chart.w)
      .attr('y', chart.h)
      .attr('dy', '15px')
      .attr('text-anchor', 'end');


    var hand = graph.append('line')
      .attr('class', 'hand')
      .attr('visibility', 'hidden')
      .attr('x1', 0)
      .attr('y1', 0)
      .attr('x2', 0)
      .attr('y2', chart.h)
      .attr('stroke-width', '1px')
      .attr('stroke', '#ccc');


    function intersectRect(el1, el2) {
      var r1 = el1.getBoundingClientRect();    //BOUNDING BOX OF THE FIRST OBJECT
      var r2 = el2.getBoundingClientRect();    //BOUNDING BOX OF THE SECOND OBJECT

      //CHECK IF THE TWO BOUNDING BOXES OVERLAP
      return !(r2.left > r1.right ||
      r2.right < r1.left ||
      r2.top > r1.bottom ||
      r2.bottom < r1.top);
    }

    var mouseover = function (d, i) {
      var activeCircles = d3.selectAll('.circle-group .circle-' + i);
      var activeOuterCircles = d3.selectAll('.circle-group .circle-' + i + '-outer');
      activeCircles.attr('r', hoverSize);
      activeOuterCircles.attr('r', expandedSize);

      hand.attr('x1', activeCircles.attr('cx'))
        .attr('x2', activeCircles.attr('cx'))
        .attr('visibility', 'visible');
      svg.selectAll('path.line')
        .attr('opacity', 0.6);
      var tick = svg.select('.ticks-' + i)
        .attr('visibility', 'visible');

      if (intersectRect(tick[0][0], xAxisLabelFrom[0][0])) {
        xAxisLabelFrom.attr('visibility', 'hidden');
      }
      else {
        xAxisLabelFrom.attr('visibility', 'visible');
      }
      if (intersectRect(tick[0][0], xAxisLabelTo[0][0])) {
        xAxisLabelTo.attr('visibility', 'hidden');
      }
      else {
        xAxisLabelTo.attr('visibility', 'visible');
      }

      // Update tooltips' texts.
      tooltips.selectAll('text')
        .text(function (td, ti) {
          return d[key.indexOf(td) + 1];
        });
      tooltips = tooltips.sort(function (a, b) {
        var ai = key.indexOf(a) + 1,
          bi = key.indexOf(b) + 1;
        return d3.descending(d[ai], d[bi]);
      });
      tooltips
        .attr('transform', function (d, i) {
          return 'translate(0, ' + (i * 20) + ')';
        });
    };
    var mouseout = function (d, i) {
      var activeCircles = d3.selectAll('.circle-group .circle-' + i);
      var activeOuterCircles = d3.selectAll('.circle-group .circle-' + i + '-outer');
      activeCircles.attr('r', defaultSize);
      activeOuterCircles.attr('r', defaultSize);

      hand.attr('visibility', 'hidden');
      svg.selectAll('path.line')
        .attr('opacity', 1);
      svg.select('.ticks-' + i)
        .attr('visibility', 'hidden');

      xAxisLabelFrom.attr('visibility', 'visible');
      xAxisLabelTo.attr('visibility', 'visible');
    };

    var data = (key.map(function (value, index) {
      return rows.map(function (d, i) {
        labels[i] = d[0];
        return {x: i, y: +d[index + 1], index: index};
      });
    }));

    // LINES
    var linesWrapper = graph.append('g')
      .attr('class', 'lines-wrapper');
    linesWrapper.selectAll("path.line")
      .data(data)
      .enter().append("path")
      .attr("class", function (d, i) {
        return "line " + "line-" + i;
      })
      .style("stroke", function (d, i) {
        return d3.rgb(z(i));
      })
      .style("stroke-width", 2)
      .attr("d", d3.svg.line()
        .x(function (d, i) {
          return x(i);
        })
        .y(function (d) {
          return y(d.y);
        }));

    // Creates a container for each group of circles.
    var circleGroupWrapper = graph.append('g')
      .attr('class', 'circle-group-warpper');
    var circles = circleGroupWrapper.selectAll("g.circles")
      .data(data)
      .enter().append("g")
      .attr('class', function (d, i) {
        return 'circle-group ' + 'circle-group-' + i;
      })
      .attr("fill", function (d, i) {
        return d3.rgb(z(i));
      });

    // Container for each circle to have a outer circle, a main one, and a rollover circle.
    var circle = circles.selectAll('g')
      .data(function (d) {
        return d;
      })
      .enter().append('g');

    var defaultSize = 0;
    var hoverSize = 3;
    var expandedSize = 6;
    var sensitiveSize = 100;
    // This is the outer circle that is not visible on init.
    circle.append('circle')
      .attr('class', function (d, i) {
        return 'circle-outer circle-' + i + '-outer';
      })
      .attr("cx", function (d, i) {
        return x(i);
      })
      .attr("cy", function (d, i) {
        return y(d.y);
      })
      .attr('fill-opacity', 0.2)
      .attr("r", defaultSize);

    circle.append('circle')
      .attr("class", function (d, i) {
        return 'circle circle-' + i;
      })
      .attr("cx", function (d, i) {
        return x(i);
      })
      .attr("cy", function (d, i) {
        return y(d.y);
      })
      .attr("r", defaultSize);

    circle.append('clipPath').attr("id", function (d, i) {
      return "clip-" + d.index + "-" + i;
    }).append('circle')
      .attr("class", function (d, i) {
        return 'circle-mouse circle-' + i + '-mouse';
      })
      .attr("fill", "#000000")
      .attr("color", "#000000")
      .attr("cx", function (d, i) {
        return x(d.x);
      })
      .attr("cy", function (d, i) {
        return y(d.y);
      })
      .attr("r", sensitiveSize)
      .on("mouseover", mouseover)
      .on("mouseout", mouseout);

    var hoverBarWidth = chart.w / (rows.length - 1);
    graph.append('g')
      .attr('class', 'hoverbar')
      .attr('opacity', 0.0)
      .selectAll('g.hoverbar').data(rows)
      .enter().append('rect')
      .attr('style', 'stroke: white;')
      .attr('x', function (d, i) {
        var x = i * hoverBarWidth;
        if (i != 0) {
          x -= hoverBarWidth / 2;
        }
        return x;
      })
      .attr('y', 0)
      .attr('width', function (d, i) {
        var width = hoverBarWidth;
        if (i == 0 || i == rows.length - 1) {
          width /= 2;
        }
        return width;
      })
      .attr('height', chart.h)
      .on('mouseover', function (d, i) {
        mouseover(d, i);
      })
      .on('mouseout', function (d, i) {
        mouseout(d, i);
      });

    /* X AXIS */
    var xTicks = graph.selectAll("g.ticks")
      .data(x.ticks(rows.length - 1))
      .enter().append("g")
      .attr("class", function (d, i) {
        return "ticks ticks-" + i;
      })
      .attr('visibility', 'hidden')
      .attr('transform', function (d, i) {
        return 'translate(' + x(d) + ',' + (chart.h) + ')'
      });

    xTicks.append('text')
      .text(function (d, i) {
        return labels[i];
      })
      .attr("text-anchor", "middle")
      .attr('dx', function (d, i) {
        var thisRect = this.getBoundingClientRect(),
          graphRect = graph.select('.lines-wrapper')[0][0].getBoundingClientRect(),
          left = graphRect.left - thisRect.left,
          right = graphRect.right - thisRect.right;
        if (left > 0) {
          return left;
        }
        if (right < 0) {
          return right;
        }
        return 0;
      })
      .attr('dy', '15px');

    var xTicksPadding = 2;
    xTicks.each(function (i) {
      var el = d3.select(this);
      bBox = this.getBBox();
      el.insert('rect', ':first-child')
        .attr('opacity', 0)
        .attr('x', bBox.x - xTicksPadding * 3)
        .attr('y', bBox.y - xTicksPadding)
        .attr('width', bBox.width + xTicksPadding * 3 * 2)
        .attr('height', bBox.height + xTicksPadding)
        .attr('fill', 'white');
    });

    /* LEGEND */
    var legend = svg.append("g")
      .attr("class", "legend")
      .attr('width', legend.w)
      .attr("transform", "translate(" + (w - legend.w) + "," + 0 + ")");

    var keys = legend.selectAll("g")
      .data(key)
      .enter().append("g")
      .attr("transform", function (d, i) {
        return "translate(0," + d3.tileText(d, 45) + ")"
      });

    keys.append("rect")
      .attr("fill", function (d, i) {
        return d3.rgb(z(i));
      })
      .attr("width", 16)
      .attr("height", 16)
      .attr("y", 0)
      .attr("x", 0)
      .on('mouseover', function (d, i) {
        var group = graph.select('g.circle-group-' + i);
        group.selectAll('g').select('.circle').attr('r', hoverSize);

        svg.selectAll('path.line').attr('opacity', 0.4);
        svg.select('path.line' + i).attr('opacity', 1).moveToFront();
      })
      .on('mouseout', function (d, i) {
        var group = graph.select('g.circle-group-' + i);
        group.selectAll('g').select('.circle').attr('r', defaultSize);
        svg.selectAll('path.line').attr('opacity', 1);
      });

    var labelWrapper = keys.append("g");

    labelWrapper.selectAll("text")
      .data(function (d, i) {
        return d3.splitString(key[i], 45);
      })
      .enter().append("text")
      .text(function (d, i) {
        return d
      })
      .attr("x", 20)
      .attr("y", function (d, i) {
        return i * 20
      })
      .attr("dy", "1em");

    /**
     * Sets the position of the tooltip.
     * @param mouse
     *   The position of the mouse. It can be relative to a specific element.
     */
    function setTooltipPositon(mouse) {
      var x = mouse[0],
        y = mouse[1];
      if (mouse[0] > chart.w - tooltipBox.attr('width')) {
        x = x - tooltipBox.attr('width') - 10;
      }
      else {
        x += 10;
      }
      if (mouse[1] > chart.h - tooltipBox.attr('height')) {
        y = y - tooltipBox.attr('height') - 10;
      }
      else {
        y += 10;
      }
      tooltipWrapper.attr('transform', 'translate(' + x + ',' + y + ')');
    }

    var tooltipWrapper = graph.append('g')
      .attr('visibility', 'hidden')
      .attr('class', 'graph-tooltip');
    var tooltipBox = tooltipWrapper.append("rect")
      .attr('opacity', 0.75)
      .attr('fill', 'white')
      .attr('stroke', '#ccc')
      .attr('stroke-width', '1')
      .attr('x', 0)
      .attr('y', 0)
      .attr('width', 70)
      .attr('height', key.length * 20);
    var tooltips = tooltipWrapper.selectAll('g')
      .data(key)
      .enter().append('g')
      .attr('transform', function (d, i) {
        return 'translate(0, ' + (i * 20) + ')';
      })
      .attr('class', function (d, i) {
        return 'tooltip-' + i;
      });

    tooltips.append('circle')
      .attr('cx', 10)
      .attr('cy', function (d, i) {
        return 10;
      })
      .attr('r', 6)
      .attr('fill', function (d, i) {
        return d3.rgb(z(i));
      });


    tooltips.append('text')
      .text('0')
      .attr('x', 20)
      .attr('y', 10)
      .attr('dy', '0.3em')
      .attr('text-anchor', 'start');

    graph.on('mousemove', function () {
      setTooltipPositon(d3.mouse(this));
    }).on('mouseover', function () {
      tooltipWrapper.attr('visibility', 'visible');
      setTooltipPositon(d3.mouse(this));
    }).on('mouseout', function () {
      tooltipWrapper.attr('visibility', 'hidden');
    });

    function maxValue(rows) {
      var data = jQuery.extend(true, [], rows);
      data = d3.merge(data);
      var max = d3.max(data.map(function (d) {
        return +d;
      }));
      return max;
    }

  }
})(jQuery);
