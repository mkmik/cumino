Card = (name, kind) ->
  @name = name
  @kind = kind
  @group = Math.floor(Math.random()*4)
  @size = Math.floor(Math.random()*3)
  null

viewModel = {
  test: ko.observable("test value")

  phys: new Card(name, "phy") for name in ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"]
  vms: new Card("v " + name, "vm") for name in ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14"]


  modes: ["Physical machines", "Virtual machines", "Storage Machines", "Storage volumes"]

  selectedMode: ko.observable("Physical machines")
}

viewModel.cards = ko.observableArray(viewModel.phys)
viewModel.currentCards = ko.dependentObservable ( ->
  if(this.selectedMode() == "Physical machines")
    res = this.phys
  else if(this.selectedMode() == "Virtual machines")
    res = this.vms
  ), viewModel

window.viewModel = viewModel

isotopize = (el) ->
  el.isotope {
    itemSelector : '.card'
    layoutMode : 'fitRows'
    #  filter: ".card-kind-phy"
    getSortData: {
      group: (elem) -> elem.attr("data-group")
      size: (elem) -> elem.attr("data-size")
    }
  }


ko.bindingHandlers.isotope = {
    init: (element, valueAccessor, allBindingsAccessor, viewModel) ->
     # I would like to initialize isotope here, but it doesn't work
    update: (element, valueAccessor, allBindingsAccessor, viewModel) ->
      dummy = valueAccessor()() # trigger dependency
      if($(element).hasClass("isotope"))
        $(element).isotope "destroy"
        isotopize($(element))
}


ko.applyBindings viewModel

ko.linkObservableToUrl(viewModel.selectedMode, "mode", "Physical machines")


isotopize($("#cards"))
window.isotopize = isotopize

$("#sortByOriginal").click -> $("#cards").isotope {sortBy: "original-order"}
$("#sortByGroup").click -> $("#cards").isotope {sortBy: "group"}
$("#sortBySize").click -> $("#cards").isotope {sortBy: "size"}

